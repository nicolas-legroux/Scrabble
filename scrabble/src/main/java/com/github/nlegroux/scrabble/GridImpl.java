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
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.List;
import java.util.stream.IntStream;

final class GridImpl implements Grid {

    private static final int GRID_SIZE = 15;

    private final List<GridRowWritable> rows;

    GridImpl() {
        rows = new ArrayList<>();

        for (int i = 0; i < GRID_SIZE; i++) {
            rows.add(GridRowImpl.generateEmptyRow(i));
        }
    }

    private GridImpl(GridImpl other) {
        rows = new ArrayList<>();

        for (GridRowWritable row : other.rows) {
            rows.add(new GridRowImpl(row));
        }
    }

    @Override
    public List<GridElement> getRow(int row) {
        return rows.get(row).get();
    }

    @Override
    public Grid playWord(String word, int row, int column, Direction direction, int... wildcardIndices) {
        GridImpl updatedGrid = new GridImpl(this);

        List<GridElement> tiles = IntStream.range(0, word.length())
                .mapToObj(idx -> {
                    if (Arrays.stream(wildcardIndices).anyMatch(i -> i == idx)) {
                        return GridElement.wildcard(word.charAt(idx));
                    } else {
                        return GridElement.letter(word.charAt(idx));
                    }
                })
                .toList();

        switch (direction) {
            case HORIZONTAL -> updatedGrid.rows.get(row).put(tiles, column);
            case VERTICAL -> {
                for (int i = 0; i < word.length(); i++) {
                    updatedGrid.rows.get(row + i).put(List.of(tiles.get(i)), column);
                }
            }
        }

        return updatedGrid;
    }

    @Override
    public String toString() {
        StringBuilder sb = new StringBuilder();

        for (int i = 0; i < GRID_SIZE; i++) {
            if (i != 0) {
                sb.append('\n');
            }

            sb.append(rows.get(i));
        }

        return sb.toString();
    }

    private static final class GridRowImpl implements GridRowWritable {

        private final List<GridElement> rowElements;

        private GridRowImpl(GridElement... rowElements) {
            Preconditions.checkState(rowElements.length == GRID_SIZE);

            this.rowElements = Arrays.asList(rowElements);
        }

        private GridRowImpl(GridRowWritable other) {
            rowElements = new ArrayList<>(other.get());
        }

        @Override
        public List<GridElement> get() {
            return Collections.unmodifiableList(rowElements);
        }

        @Override
        public void put(List<GridElement> tiles, int columnOffset) {
            for (int i = 0; i < tiles.size(); i++) {
                rowElements.set(columnOffset + i, tiles.get(i));
            }
        }

        @Override
        public String toString() {
            StringBuilder sb = new StringBuilder();

            for (GridElement element : rowElements) {
                char character =
                        switch (element) {
                            case GridElement.Empty empty -> toChar(empty);
                            case GridElement.LetterGridElement letter -> letter.character();
                            case GridElement.WildcardGridElement wildcard -> wildcard.character();
                        };
                sb.append(character);
            }

            return sb.toString();
        }

        private static char toChar(GridElement.Empty empty) {
            return switch (empty) {
                case DEFAULT -> ' ';
                case DOUBLE_LETTER -> '2';
                case TRIPLE_LETTER -> '3';
                case DOUBLE_WORD -> '+';
                case TRIPLE_WORD -> '*';
            };
        }

        private static GridRowImpl generateEmptyRow(int idx) {
            return switch (idx) {
                case 0, 14 -> new GridRowImpl(
                        GridElement.Empty.TRIPLE_WORD,
                        GridElement.Empty.DEFAULT,
                        GridElement.Empty.DEFAULT,
                        GridElement.Empty.DOUBLE_LETTER,
                        GridElement.Empty.DEFAULT,
                        GridElement.Empty.DEFAULT,
                        GridElement.Empty.DEFAULT,
                        GridElement.Empty.TRIPLE_WORD,
                        GridElement.Empty.DEFAULT,
                        GridElement.Empty.DEFAULT,
                        GridElement.Empty.DEFAULT,
                        GridElement.Empty.DOUBLE_LETTER,
                        GridElement.Empty.DEFAULT,
                        GridElement.Empty.DEFAULT,
                        GridElement.Empty.TRIPLE_WORD);
                case 1, 13 -> new GridRowImpl(
                        GridElement.Empty.DEFAULT,
                        GridElement.Empty.DOUBLE_WORD,
                        GridElement.Empty.DEFAULT,
                        GridElement.Empty.DEFAULT,
                        GridElement.Empty.DEFAULT,
                        GridElement.Empty.TRIPLE_LETTER,
                        GridElement.Empty.DEFAULT,
                        GridElement.Empty.DEFAULT,
                        GridElement.Empty.DEFAULT,
                        GridElement.Empty.TRIPLE_LETTER,
                        GridElement.Empty.DEFAULT,
                        GridElement.Empty.DEFAULT,
                        GridElement.Empty.DEFAULT,
                        GridElement.Empty.DOUBLE_WORD,
                        GridElement.Empty.DEFAULT);
                case 2, 12 -> new GridRowImpl(
                        GridElement.Empty.DEFAULT,
                        GridElement.Empty.DEFAULT,
                        GridElement.Empty.DOUBLE_WORD,
                        GridElement.Empty.DEFAULT,
                        GridElement.Empty.DEFAULT,
                        GridElement.Empty.DEFAULT,
                        GridElement.Empty.DOUBLE_LETTER,
                        GridElement.Empty.DEFAULT,
                        GridElement.Empty.DOUBLE_LETTER,
                        GridElement.Empty.DEFAULT,
                        GridElement.Empty.DEFAULT,
                        GridElement.Empty.DEFAULT,
                        GridElement.Empty.DOUBLE_WORD,
                        GridElement.Empty.DEFAULT,
                        GridElement.Empty.DEFAULT);
                case 3, 11 -> new GridRowImpl(
                        GridElement.Empty.DOUBLE_LETTER,
                        GridElement.Empty.DEFAULT,
                        GridElement.Empty.DEFAULT,
                        GridElement.Empty.DOUBLE_WORD,
                        GridElement.Empty.DEFAULT,
                        GridElement.Empty.DEFAULT,
                        GridElement.Empty.DEFAULT,
                        GridElement.Empty.DOUBLE_LETTER,
                        GridElement.Empty.DEFAULT,
                        GridElement.Empty.DEFAULT,
                        GridElement.Empty.DEFAULT,
                        GridElement.Empty.DOUBLE_WORD,
                        GridElement.Empty.DEFAULT,
                        GridElement.Empty.DEFAULT,
                        GridElement.Empty.DOUBLE_LETTER);
                case 4, 10 -> new GridRowImpl(
                        GridElement.Empty.DEFAULT,
                        GridElement.Empty.DEFAULT,
                        GridElement.Empty.DEFAULT,
                        GridElement.Empty.DEFAULT,
                        GridElement.Empty.DOUBLE_WORD,
                        GridElement.Empty.DEFAULT,
                        GridElement.Empty.DEFAULT,
                        GridElement.Empty.DEFAULT,
                        GridElement.Empty.DEFAULT,
                        GridElement.Empty.DEFAULT,
                        GridElement.Empty.DOUBLE_WORD,
                        GridElement.Empty.DEFAULT,
                        GridElement.Empty.DEFAULT,
                        GridElement.Empty.DEFAULT,
                        GridElement.Empty.DEFAULT);
                case 5, 9 -> new GridRowImpl(
                        GridElement.Empty.DEFAULT,
                        GridElement.Empty.TRIPLE_LETTER,
                        GridElement.Empty.DEFAULT,
                        GridElement.Empty.DEFAULT,
                        GridElement.Empty.DEFAULT,
                        GridElement.Empty.TRIPLE_LETTER,
                        GridElement.Empty.DEFAULT,
                        GridElement.Empty.DEFAULT,
                        GridElement.Empty.DEFAULT,
                        GridElement.Empty.TRIPLE_LETTER,
                        GridElement.Empty.DEFAULT,
                        GridElement.Empty.DEFAULT,
                        GridElement.Empty.DEFAULT,
                        GridElement.Empty.TRIPLE_LETTER,
                        GridElement.Empty.DEFAULT);
                case 6, 8 -> new GridRowImpl(
                        GridElement.Empty.DEFAULT,
                        GridElement.Empty.DEFAULT,
                        GridElement.Empty.DOUBLE_LETTER,
                        GridElement.Empty.DEFAULT,
                        GridElement.Empty.DEFAULT,
                        GridElement.Empty.DEFAULT,
                        GridElement.Empty.DOUBLE_LETTER,
                        GridElement.Empty.DEFAULT,
                        GridElement.Empty.DOUBLE_LETTER,
                        GridElement.Empty.DEFAULT,
                        GridElement.Empty.DEFAULT,
                        GridElement.Empty.DEFAULT,
                        GridElement.Empty.DOUBLE_LETTER,
                        GridElement.Empty.DEFAULT,
                        GridElement.Empty.DEFAULT);
                case 7 -> new GridRowImpl(
                        GridElement.Empty.TRIPLE_WORD,
                        GridElement.Empty.DEFAULT,
                        GridElement.Empty.DEFAULT,
                        GridElement.Empty.DOUBLE_LETTER,
                        GridElement.Empty.DEFAULT,
                        GridElement.Empty.DEFAULT,
                        GridElement.Empty.DEFAULT,
                        GridElement.Empty.DOUBLE_WORD,
                        GridElement.Empty.DEFAULT,
                        GridElement.Empty.DEFAULT,
                        GridElement.Empty.DEFAULT,
                        GridElement.Empty.DOUBLE_LETTER,
                        GridElement.Empty.DEFAULT,
                        GridElement.Empty.DEFAULT,
                        GridElement.Empty.TRIPLE_WORD);
                default -> throw new IllegalArgumentException(
                        String.format("idx must be between 0 and %s", GRID_SIZE - 1));
            };
        }
    }
}
