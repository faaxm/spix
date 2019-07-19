sh devtools/run-clang-format.sh

if git diff-index --quiet HEAD --
then
    echo "Success: Clang-format ran without changing the code.";
else
    echo "Error: Please run clang-format (e.g. via devtools/run-clang-format.sh)";
    echo "Running clang-format on the buildserver introduced changes in the source which indicates it has not been run on the last commit";
    
    git diff;
    
    exit 1;
fi
