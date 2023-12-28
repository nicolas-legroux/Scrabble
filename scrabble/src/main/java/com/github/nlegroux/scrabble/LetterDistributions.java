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

final class LetterDistributions {

    private LetterDistributions() {}

    private static final LetterDistribution FRENCH_DISTRIBUTION = letter -> switch (letter) {
        case E -> 15;
        case A -> 9;
        case I -> 8;
        case N, O, R, S, T, U -> 6;
        case L -> 5;
        case D, M -> 3;
        case B, C, F, G, H, P, V, WILDCARD -> 2;
        case J, K, Q, W, X, Y, Z -> 1;
    };

    static LetterDistribution french() {
        return FRENCH_DISTRIBUTION;
    }
}
