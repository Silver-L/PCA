@echo off

set exe="F:\python_program\PCA\evaluation\x64\Release\specificity.exe"
set LSDM_pathEvect="F:\python_program\PCA\debug\evect_list.txt"
set LSDM_pathEval="F:\python_program\PCA\debug\eval_list.txt"
set LSDM_pathMean="F:\python_program\PCA\debug\mean_list.txt"
set test_eudt_list="F:\python_program\PCA\debug\eudt_list.txt"
set dirOut=F:\python_program\PCA\debug\evaluation
set LSDM_te=2
set num_gene=5

call %exe% %LSDM_pathEvect% %LSDM_pathEval% %LSDM_pathMean% %test_eudt_list% %dirOut% %LSDM_te% %num_gene%

pause