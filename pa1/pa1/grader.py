#!/usr/bin/env python3

import autograde

name = 'PA1'
release = 1

assignment = autograde.MultiProject(
    autograde.StringTests.Project('roman', weight=0.625),
    autograde.StringTests.Project('palindrome', weight=0.625),
    autograde.StringTests.Project('rle', weight=0.625),
    autograde.StdinFileTests.Project('list', weight=2),
    autograde.FileTests.Project('mexp', weight=2),
    autograde.Project('bst',
        autograde.StdinFileTests(id='1', weight=1.5),
        autograde.StdinFileTests(id='2', weight=1.5)
    )
)

if __name__ == '__main__':
    autograde.main(name, assignment, release)
