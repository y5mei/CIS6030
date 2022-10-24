# CIS6030 A3 PostgreSQL
Yaowen Mei (1177855)

## Setup PostreSQL Environment
1. Assume you already have PostgreSQL and pgAdmin installed. Then if you want to create new user and database, please fllow steps 2 to 4; Otherwise, you can jump to step 5 to use existing username and database.
2. Set up a user `admin` with password `admin` ![Create a user 'admin' with password 'admin'](resource/setup-user.png "Create User and Passoword ")
3. Set up a database name `postgres-cis6030` ![Create a database](resource/create-database.png "Create a database")
4. Set user `admin` to be the owner of the database `postgres-cis6030` ![Change Owner for the database](resource/change-owner.png "Change Owner for the database")
5. You are completely done with this step now. However, alternatively, if you do not want to create new user/database, and just want to use your existing user and database, you can do so by change the `YourConstant.py` file:
```python
USER_NAME = "admin" # change to your existing username
USER_PASSWORD = "admin" # change to your password
DATABASE_NAME = "postgres-cis6030" # change to a database that your username has ownership
```

### Setup Python Environment 
1. You can use Python 3.7. 3.8, or 3.9:
* to set-up environment with `pip3`, follow step 2 and 3; 
* to set-up environment with `conda`, please follow step 4, 5; 
2. Make sure you have `pip3` installed already; otherwise, you will need to execute the command below in your terminal:

```
sudo apt install python3-pip
```

You can verify pip3 is installed by: 

```
pip3 --version
```
3. Install all the dependency from the `requirement.txt file`

```buildoutcfg
pip3 install -r requirements.txt
```
4. setup a conda environment: `conda env create --file 6030a3.yml`
5. Inside the conda environment, if your conda has hard time to install `PrettyTable`, then you might need to manually install the `PrettyTable` dependency:
```buildoutcfg
pip install -U git+https://github.com/jazzband/prettytable
```
4. run main.py to upload all the csv rows to your database under a table called `admission`
5. run `SQLOperations.py` to perform queries on the table.
6. run `MultiVariableLinearRegression.py` to train linear regression model and make prediction.
6. run `LogisticRegression.py` to train logistic regression model and make prediction.

TODO:
2. 线性回归大于1 的， 打印出1 和 括号里 （越界的值）
3. SQL querry 的例子要多给几个
