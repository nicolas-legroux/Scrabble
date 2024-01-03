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

sealed interface GridElement {

    /**
     * When no tile has been placed on a given row/column.
     */
    enum Empty implements GridElement {
        DEFAULT,
        DOUBLE_LETTER,
        TRIPLE_LETTER,
        DOUBLE_WORD,
        TRIPLE_WORD;
    }

    /**
     * When a regular letter tile has been placed on a given row/column.
     */
    record LetterGridElement(char character) implements GridElement {}

    /**
     * When a wildcard tile has been placed on a given row/column.
     */
    record WildcardGridElement(char character) implements GridElement {}

    static GridElement letter(char character) {
        return new LetterGridElement(character);
    }

    static GridElement wildcard(char character) {
        return new WildcardGridElement(character);
    }
}
