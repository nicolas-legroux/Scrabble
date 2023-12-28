/*
 * (c) Copyright 2023 Nicolas Legroux. All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package com.github.nlegroux.scrabble;

import static org.assertj.core.api.Assertions.assertThat;
import static org.assertj.core.api.Assertions.assertThatThrownBy;

import com.google.common.collect.ImmutableMultiset;
import com.google.common.collect.Multiset;
import java.util.stream.Stream;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.MethodSource;

final class BagImplTest {

    @Test
    void bag_threeElements() {
        LetterDistribution distribution = letter -> {
            if (letter.equals(ScrabbleLetter.A)) {
                return 3;
            }

            return 0;
        };

        Bag bag = new BagImpl(distribution);
        assertThat(bag.size()).isEqualTo(3);

        assertThat(bag.draw(2)).isEqualTo(ImmutableMultiset.of(ScrabbleLetter.A, ScrabbleLetter.A));
        assertThat(bag.size()).isEqualTo(1);
        assertThat(bag.draw(2)).isEqualTo(ImmutableMultiset.of(ScrabbleLetter.A));
        assertThat(bag.size()).isEqualTo(0);
    }

    @Test
    void bag_frenchDistribution() {
        LetterDistribution distribution = LetterDistributions.french();

        Bag bag = new BagImpl(distribution);

        assertThat(bag.isEmpty()).isFalse();

        ImmutableMultiset.Builder<ScrabbleLetter> builder = ImmutableMultiset.builder();

        while (!bag.isEmpty()) {
            builder.addAll(bag.draw(7));
        }

        assertThat(bag.isEmpty()).isTrue();

        Multiset<ScrabbleLetter> letters = builder.build();

        for (ScrabbleLetter letter : ScrabbleLetter.values()) {
            assertThat(distribution.getDistribution(letter)).isEqualTo(letters.count(letter));
        }
    }

    @Test
    void bag_randomizes() {
        LetterDistribution distribution = LetterDistributions.french();

        Bag bag1 = new BagImpl(distribution);
        Bag bag2 = new BagImpl(distribution);

        assertThat(bag1.draw(7)).isNotEqualTo(bag2.draw(7));
    }

    @ParameterizedTest
    @MethodSource("numberOfTilesToDraw")
    void bag_drawsRequestedNumberOfTiles(int numberOfTilesToDraw) {
        Bag bag = new BagImpl(LetterDistributions.french());

        assertThat(bag.draw(numberOfTilesToDraw)).hasSize(numberOfTilesToDraw);
    }

    private static Stream<Integer> numberOfTilesToDraw() {
        return Stream.of(1, 3, 7);
    }

    @Test
    void bag_throws_ifRequestingIllegalNumberOfTiles() {
        Bag bag = new BagImpl(LetterDistributions.french());

        assertThat(bag.size()).isEqualTo(102);

        assertThatThrownBy(() -> bag.draw(-1))
                .isExactlyInstanceOf(IllegalArgumentException.class)
                .hasMessage("numTiles must be positive");
        assertThatThrownBy(() -> bag.draw(0))
                .isExactlyInstanceOf(IllegalArgumentException.class)
                .hasMessage("numTiles must be positive");

        assertThatThrownBy(() -> bag.draw(8))
                .isExactlyInstanceOf(IllegalArgumentException.class)
                .hasMessage("At most 7 tiles can be drawn");

        assertThat(bag.size()).isEqualTo(102);
    }
}
