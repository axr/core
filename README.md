The AXR Project
===============
The AXR Project is a revolutionary open source project that will fundamentally
improve our tools to create web sites and apps.

It's much in the style of current web standards, using plain text files linked
together to create the final site, but with a complete separation of content and
presentation, where the content is pure semantic data written in XML, and the
presentation and behavior layer is written in HSS, a new language based on CSS,
but with many powerful features, such as hierarchical notation, object
orientation, modularization for code reuse, expressions (math), vector graphics,
references (bindings between objects), and a very long etc.

Prototype
=========
The Prototype is an application where the  written in C++. It uses Expat for
parsing XML and Cairo for rendering the graphics. It serves as a testing ground
and for developing the initial draft of the core library that will power the AXR
platform.

Repository manager
===================
This repository is managed by Miro Keller (@veosotano on github,
veosotano@gmail.com). Please read through the entire readme before contacting
him, since this may answer your question.

Find something to do
====================
You can find work on the github issues tracker: https://github.com/axr/prototoype/issues.
If there is an unassigned or an old task, post a comment asking for assignment.
Make sure you follow the coding standards and our workflow, which you can find
below.

The workflow
============
If you are not used to git, go ahead and read more here: http://help.github.com/
Relevant to our workflow: http://help.github.com/fork-a-repo/, http://help.github.com/send-pull-requests/

1. Go ahead and fork the AXR/Prototype repo. You will use that repository as
your working repo.
2. You can do any git changes you like to it.
3. When you finish, go to your fork and send a pull request. We'll review it as
soon as possible. After it has been reviewed and accepted, we will merge it into
our repo.

Coding standards
================
Follow these coding standards when writing code for the prototype. The rules are
quite simple and they ensure the code stays clean and readable. In general,
follow the style as the code that is already present, but here are some
guidelines:

- Variable names: in camelcase starting with a lowercase character.
- Class names: (do you have more things in C++?) in camelcase starting with a
  capital character.
- Namespace: all the classes in the core library should be in the AXR namespace.
  Wrappers and classes outside of the library should use another one.
- Whitespace:
  - Indent with 4 spaces.
  - Use Unix linebreaks.
- Line breaking:
    - Each statement should go on its own line.
    - An "else" statement should go on the same line as the preceding closing
      brace.
    - Each brace on functions or methods should go on their own line.
    - The opening brace on "if" statements go on the same line.

Building
========
Read our [wiki page](http://axr.vg/wiki/Building).

Pre-commit hook
===============
It is reccommended that you use our pre-commit hook. In order to do so, simply
copy or symlink the executable at bin/pre-commit to .git/hooks/

Where to file bug reports
=========================
Please create a new issue here: https://github.com/AXR/Prototype/issues/new. To
make the process of fixing bugs easier, developers need to know what causes the
problem and also on what platform it happens. Be as detailed as possible, and
make sure to include the following points in your bug report:

    - What is happening.
    - What did you expect to happen.
    - How to reproduce.
    - Your operating system's version.
    - Which version of the prototype is affected.
