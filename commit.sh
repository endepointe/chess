#!/usr/bin/bash
#branch=$(git branch)
#echo "$branch"
git add -A && git commit -m $1 && git push origin $2 
