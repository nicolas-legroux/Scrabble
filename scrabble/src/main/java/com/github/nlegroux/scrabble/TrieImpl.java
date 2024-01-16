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
import java.util.Collections;
import java.util.HashMap;
import java.util.Map;
import java.util.Optional;

final class TrieImpl implements Trie {

    private final TrieNode root;

    TrieImpl(TrieNode root) {
        this.root = Preconditions.checkNotNull(root, "root must not be null");
    }

    @Override
    public boolean isPrefix(String prefix) {
        return getNodeForPrefix(prefix).isPresent();
    }

    @Override
    public boolean isWord(String word) {
        return getNodeForPrefix(word).map(TrieNode::isEndOfWord).orElse(false);
    }

    private Optional<TrieNode> getNodeForPrefix(String prefix) {
        TrieNode current = root;

        for (int i = 0; i < prefix.length(); i++) {
            char character = prefix.charAt(i);

            TrieNode next = current.edges().get(character);

            if (next == null) {
                return Optional.empty();
            }

            current = next;
        }

        return Optional.of(current);
    }

    private static final class TrieNode {

        private final Map<Character, TrieNode> edges;

        private boolean isEndOfWord;

        TrieNode() {
            edges = new HashMap<>();
            isEndOfWord = false;
        }

        TrieNode addEdge(char character) {
            TrieNode existingNode = edges.get(character);

            if (existingNode != null) {
                return existingNode;
            }

            TrieNode newNode = new TrieNode();
            edges.put(character, newNode);

            return newNode;
        }

        void markEndOfWord() {
            isEndOfWord = true;
        }

        boolean isEndOfWord() {
            return isEndOfWord;
        }

        Map<Character, TrieNode> edges() {
            return Collections.unmodifiableMap(edges);
        }
    }

    static Builder builder() {
        return new Builder();
    }

    static final class Builder {

        private final TrieNode root;

        Builder() {
            this.root = new TrieNode();
        }

        Trie build() {
            return new TrieImpl(root);
        }

        Builder addWord(String word) {
            TrieNode current = root;

            for (int i = 0; i < word.length(); i++) {
                char character = word.charAt(i);

                current = current.addEdge(character);
            }

            current.markEndOfWord();

            return this;
        }
    }
}
