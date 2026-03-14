#!/usr/bin/env bash
set -e

MSG="commit"

# git リポジトリか確認
if ! git rev-parse --is-inside-work-tree >/dev/null 2>&1; then
  echo "ここは git リポジトリではありません"
  exit 1
fi

# 変更追加
git add .

# 変更があればコミット
if ! git diff --cached --quiet; then
  git commit -m "$MSG"
else
  echo "変更なし"
fi

# 現在のブランチ取得
BRANCH=$(git rev-parse --abbrev-ref HEAD)

# force push
git push --force origin "$BRANCH"