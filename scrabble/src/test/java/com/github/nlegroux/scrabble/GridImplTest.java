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

import com.github.nlegroux.scrabble.Grid.Direction;
import com.github.nlegroux.scrabble.GridElement.Empty;
import org.junit.jupiter.api.Test;

final class GridImplTest {

    @Test
    void grid_empty_toString() {
        Grid grid = new GridImpl();

        assertThat(grid.toString())
                .isEqualTo("*  2   *   2  *\n"
                        + " +   3   3   + \n"
                        + "  +   2 2   +  \n"
                        + "2  +   2   +  2\n"
                        + "    +     +    \n"
                        + " 3   3   3   3 \n"
                        + "  2   2 2   2  \n"
                        + "*  2   +   2  *\n"
                        + "  2   2 2   2  \n"
                        + " 3   3   3   3 \n"
                        + "    +     +    \n"
                        + "2  +   2   +  2\n"
                        + "  +   2 2   +  \n"
                        + " +   3   3   + \n"
                        + "*  2   *   2  *");
    }

    @Test
    void playWord_horizontal() {
        Grid grid = new GridImpl();
        Grid updatedGrid = grid.playWord("ABCDEF", 1, 4, Direction.HORIZONTAL);

        assertThat(updatedGrid.getRow(1))
                .containsExactly(
                        Empty.DEFAULT,
                        Empty.DOUBLE_WORD,
                        Empty.DEFAULT,
                        Empty.DEFAULT,
                        GridElement.letter('A'),
                        GridElement.letter('B'),
                        GridElement.letter('C'),
                        GridElement.letter('D'),
                        GridElement.letter('E'),
                        GridElement.letter('F'),
                        Empty.DEFAULT,
                        Empty.DEFAULT,
                        Empty.DEFAULT,
                        Empty.DOUBLE_WORD,
                        Empty.DEFAULT);
        assertThat(grid.getRow(1))
                .containsExactly(
                        Empty.DEFAULT,
                        Empty.DOUBLE_WORD,
                        Empty.DEFAULT,
                        Empty.DEFAULT,
                        Empty.DEFAULT,
                        Empty.TRIPLE_LETTER,
                        Empty.DEFAULT,
                        Empty.DEFAULT,
                        Empty.DEFAULT,
                        Empty.TRIPLE_LETTER,
                        Empty.DEFAULT,
                        Empty.DEFAULT,
                        Empty.DEFAULT,
                        Empty.DOUBLE_WORD,
                        Empty.DEFAULT);
    }

    @Test
    void playWord_vertical() {
        Grid grid = new GridImpl();
        Grid updatedGrid = grid.playWord("ABCDEF", 1, 4, Direction.VERTICAL);

        assertThat(updatedGrid.getRow(1))
                .containsExactly(
                        Empty.DEFAULT,
                        Empty.DOUBLE_WORD,
                        Empty.DEFAULT,
                        Empty.DEFAULT,
                        GridElement.letter('A'),
                        Empty.TRIPLE_LETTER,
                        Empty.DEFAULT,
                        Empty.DEFAULT,
                        Empty.DEFAULT,
                        Empty.TRIPLE_LETTER,
                        Empty.DEFAULT,
                        Empty.DEFAULT,
                        Empty.DEFAULT,
                        Empty.DOUBLE_WORD,
                        Empty.DEFAULT);
        assertThat(updatedGrid.getRow(2).get(4)).isEqualTo(GridElement.letter('B'));
        assertThat(updatedGrid.getRow(3).get(4)).isEqualTo(GridElement.letter('C'));
        assertThat(updatedGrid.getRow(4).get(4)).isEqualTo(GridElement.letter('D'));
        assertThat(updatedGrid.getRow(5).get(4)).isEqualTo(GridElement.letter('E'));
        assertThat(updatedGrid.getRow(6).get(4)).isEqualTo(GridElement.letter('F'));
        assertThat(updatedGrid.getRow(7).get(4)).isEqualTo(Empty.DEFAULT);

        assertThat(grid.getRow(1))
                .containsExactly(
                        Empty.DEFAULT,
                        Empty.DOUBLE_WORD,
                        Empty.DEFAULT,
                        Empty.DEFAULT,
                        Empty.DEFAULT,
                        Empty.TRIPLE_LETTER,
                        Empty.DEFAULT,
                        Empty.DEFAULT,
                        Empty.DEFAULT,
                        Empty.TRIPLE_LETTER,
                        Empty.DEFAULT,
                        Empty.DEFAULT,
                        Empty.DEFAULT,
                        Empty.DOUBLE_WORD,
                        Empty.DEFAULT);
    }

    @Test
    void playWord_withWildcard() {
        Grid grid = new GridImpl();

        int[] wildcardIndices = new int[] {0, 2};
        Grid updatedGrid = grid.playWord("ABC", 1, 4, Direction.HORIZONTAL, wildcardIndices);

        assertThat(updatedGrid.getRow(1))
                .containsExactly(
                        Empty.DEFAULT,
                        Empty.DOUBLE_WORD,
                        Empty.DEFAULT,
                        Empty.DEFAULT,
                        GridElement.wildcard('A'),
                        GridElement.letter('B'),
                        GridElement.wildcard('C'),
                        Empty.DEFAULT,
                        Empty.DEFAULT,
                        Empty.TRIPLE_LETTER,
                        Empty.DEFAULT,
                        Empty.DEFAULT,
                        Empty.DEFAULT,
                        Empty.DOUBLE_WORD,
                        Empty.DEFAULT);
    }
}
