
clang-format --version
sh devtools/run-clang-format.sh

if git diff-index --quiet HEAD --
then
    echo "Success: Clang-format ran without changing the code.";
else
    echo;
    echo;
    echo "################################################################################";
    echo "Error: Please run clang-format (e.g. via devtools/run-clang-format.sh)";
    echo;
    echo "Running clang-format on the buildserver introduced changes in the source";
    echo "which indicates it has not been run on the last commit. Please fix the formatting";
    echo "by running clang-format locally and committing the changes.";
    echo "################################################################################";
    echo;
    echo;

    git diff;

    exit 1;
fi
