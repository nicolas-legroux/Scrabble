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

import com.google.common.base.Preconditions;
import com.google.common.collect.ImmutableMultiset;
import com.google.common.collect.Multiset;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

final class BagImpl implements Bag {

    private final List<ScrabbleLetter> letters;

    private int cursor;

    BagImpl(LetterDistribution letterDistribution) {
        letters = new ArrayList<>();

        for (ScrabbleLetter letter : ScrabbleLetter.values()) {
            int numTilesForLetter = letterDistribution.getDistribution(letter);

            for (int i = 0; i < numTilesForLetter; i++) {
                letters.add(letter);
            }
        }

        Collections.shuffle(letters);

        cursor = 0;
    }

    @Override
    public Multiset<ScrabbleLetter> draw(int numTiles) {
        Preconditions.checkArgument(numTiles >= 1, "numTiles must be positive");
        Preconditions.checkArgument(numTiles <= 7, "At most 7 tiles can be drawn");

        int numTilesToDraw = Math.min(numTiles, size());

        ImmutableMultiset.Builder<ScrabbleLetter> builder = ImmutableMultiset.builder();

        for (int i = 0; i < numTilesToDraw; i++) {
            builder.add(letters.get(cursor + i));
        }

        cursor += numTilesToDraw;

        return builder.build();
    }

    @Override
    public int size() {
        int size = letters.size() - cursor;

        Preconditions.checkState(size >= 0, "Size cannot be negative");

        return size;
    }
}
