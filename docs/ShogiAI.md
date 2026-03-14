# ShogiAI

## AGENTS.md 概要
- ニューラルネットを使った将棋AI（C++20 + LibTorch）。
- モジュール分割で段階的に実装できる設計。
- 実装順序は「盤面→指し手生成→合法手判定→ゲーム管理→自己対局→NN評価→MCTS→詰み探索」。
- 盤面はビットボード、指し手は USI 互換の `Move {from,to,piece,promote,drop}`。
- 規約は `#pragma once`、命名は `ClassName / snake_case`、STLは `vector/array/optional/span` のみ使用。
- CMake でビルド、`ctest` でテスト。
- 指し手生成は性能最優先（ビット演算/ルックアップ/マジックビットボード等）。
- NN は Conv + ResBlock + Policy/Value、CPU/CUDA 対応。
- 変更時はビルド維持・テスト更新・インターフェース維持・モジュール独立を守る。
- 許可タスクは実装/リファクタ/テスト/性能改善。アーキテクチャ変更は禁止。

## 進捗状況 (2026-03-14)
- 盤面表現: `Bitboard` と `Board` を実装。SFEN 読み込み、初期局面設定、`piece_at` による盤上検索が可能。
- 指し手表現: USI 指し手の解析/生成（通常指し手と打ち）。`parse_usi_move` は局面から `from` の駒種を取得。
- USI エンジン骨格: `usi/isready/position/go/quit` に応答。`position startpos/sfen` を反映し、`moves` を簡易適用（捕獲・成り・打ち・手駒更新）。`go` は未実装で `bestmove resign`。
- NN 入力: 盤面エンコード（駒 28ch + 手駒 14ch + 手番 1ch = 43ch）。

## 未着手/不足
- 指し手生成、合法手判定、ゲーム管理、自己対局、学習、探索（MCTS/詰み）。
- テスト未整備（`tests/` 不在、`ctest` 未対応）。
- ルール詳細（王手判定や合法性チェック等）が未実装のため、現状は手の正当性を保証できない。
