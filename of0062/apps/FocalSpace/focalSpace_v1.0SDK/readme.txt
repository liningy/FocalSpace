PUSH YOUR CHANGE

< for the first time, copy the entire folder to your local directory>
$ git clone git@github.com: liningy/FocalSpace.git

$ cd of0062
$ git add of0062
$ git commit -m �what have been changed�

<add remote origion, this is only needed to be done once>
$ git remote add origin git@github.com: liningy/FocalSpace.git

<confirm pushing>
$ git push origin master


FETCH UPDATES
git fetch
git merge origin/master

merge help:
http://learn.github.com/p/branching.html

SWITCH BRANCHES
git checkout branchname

GO BACK TO OLD COMMITS
(clone the entire git folder , or the newest information on git firstly)
git checkout theCommitNumber
(when you switch from different commit, if it meets slash, you could do:)
git slash save
