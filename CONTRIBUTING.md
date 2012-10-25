Find something to do
====================
You can find work on the GitHub issue trackers:

 * https://github.com/axr/browser/issues
 * https://github.com/axr/core/issues
 * https://github.com/axr/examples/issues
 * https://github.com/axr/extras/issues
 * https://github.com/axr/framework/issues
 * https://github.com/axr/plugin/issues
 * https://github.com/axr/specification/issues
 * https://github.com/axr/website/issues

If there is an unassigned or old task, post a comment asking for assignment.
Make sure you follow the coding standards and our workflow, which you can find
below.

The workflow
============
If you are not used to Git, go ahead and read more here: http://help.github.com/

Also relevant to our workflow:

 * http://help.github.com/fork-a-repo/
 * http://help.github.com/send-pull-requests/

1. Go ahead and fork the AXR repository you'd like to work on (browser, core,
examples, etc.). You will use that repository as your working repo.
2. You can make any changes you like to it.
3. When you finish, go to your fork and send a pull request. We'll review it as
soon as possible. After it has been reviewed and accepted, we will merge it into
the official repository.

Coding standards
================
Follow these coding standards when writing code for the prototype. The rules are
quite simple and they ensure the code stays clean and readable. In general,
follow the same style as the code that is already present, but here are some
guidelines:

**Please note: these are somewhat out of date and will be updated soon**

### For C, C++, and Objective-C:

- Variable and method names: in camelCase starting with a lowercase letter.
- Class names: in CamelCase starting with a capital letter.
- Namespace: all the classes in the core library should be in the AXR namespace.
  Wrappers and classes outside of the library should use another one.
- Whitespace:
  - Indent with 4 spaces.
  - Use Unix linebreaks (\n).
- Line breaks:
  - Each statement should go on its own line.
  - Curly braces go on their own lines

Pre-commit hook
===============
It is recommended that you use our pre-commit hook. The hook checks for a
variety of whitespace issues and other style problems with your code and will
notify you of any violations before your commit is made so you can fix them.

To use the hook, simply copy or symlink the executable at bin/pre-commit to
.git/hooks/, for example:

    ln -s ../../bin/pre-commit "$(git rev-parse --show-toplevel)/.git/hooks/pre-commit"
