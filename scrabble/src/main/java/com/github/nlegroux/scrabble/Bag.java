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

import com.google.common.collect.Multiset;

/**
 * A bag of letter tiles, from which tiles are randomly drawn.
 */
interface Bag {

    /**
     * Draws up to {@code numTiles} tiles from the bag. If there are less than {@code numTiles} tiles left
     * in the bag, these are returned and the bag is now empty.
     */
    Multiset<ScrabbleLetter> draw(int numTiles);

    /**
     * Returns the number of tiles left in the bag.
     */
    int size();

    default boolean isEmpty() {
        return size() == 0;
    }
}
