# CIS6030 A3 PostgreSQL
Yaowen Mei (1177855)

## Setup Environment
1. Assume you already have a PostgreSQL database running with a database name "postgres", and a user "cis6030" with a password "cis6030"
2. setup a conda environment: `conda env create --file 6030a3.yml`
3. Inside of the conda environment, you might need to manually install the `PrettyTable` dependency:
```buildoutcfg
pip install -U git+https://github.com/jazzband/prettytable
```
4. run main.py to upload all the csv rows to your database under a table called `admission`
5. run `SQLOperations.py` to perform queries on the table.
6. run `MultiVariableLinearRegression.py` to train linear regression model and make prediction.
6. run `LogisticRegression.py` to train logistic regression model and make prediction.