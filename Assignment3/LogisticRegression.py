import collections

import psycopg2
from psycopg2.extensions import ISOLATION_LEVEL_AUTOCOMMIT
import pandas as pd
from sklearn import linear_model
from sklearn.linear_model import LogisticRegression
from sklearn.datasets import load_iris
import matplotlib.pyplot as plt
from sklearn.model_selection import train_test_split
from sklearn.metrics import r2_score, accuracy_score, confusion_matrix
from sklearn.metrics import mean_squared_error
from sklearn.model_selection import cross_val_predict
from prettytable import PrettyTable
from sklearn.utils import column_or_1d
from YourConstant import *


# get data from database as pandas df
def get_dataframe_from_database():
    # read all the data from the database to build linear regression
    con = psycopg2.connect(f"host=localhost dbname={DATABASE_NAME} user={USER_NAME} password={USER_PASSWORD}")
    cursor = con.cursor()
    cursor.execute("""SELECT COLUMN_NAME  FROM INFORMATION_SCHEMA.COLUMNS WHERE TABLE_NAME = N'admission'""")
    col_names = [_[0] for _ in cursor.fetchall()]

    cursor = con.cursor()
    cursor.execute("""SELECT * FROM admission""")
    all_records = cursor.fetchall()

    df = pd.DataFrame(all_records, columns=col_names)
    return df


def get_cal_names_for_user_input(df):
    col_names = list(df.columns)
    col_names = col_names[1:-1]
    return col_names


# use df and the user input record to predict the admission rate
def train_model(df):
    # there is no need to re-normalize the inputs for regression
    X = df.iloc[:, 1:8]
    y = df.iloc[:, 8:9]
    col_names = get_cal_names_for_user_input(df)

    y["Chance_of_Admit_"] = y["Chance_of_Admit_"].apply(lambda x: True if x > 0.5 else False)

    X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.2,
                                                        random_state=19)  # 20% data were used for testing, 80% data were used for training
    # define the multinomial logistic regression model
    regr = LogisticRegression(random_state=12, solver='lbfgs', max_iter=1000).fit(X_train, y_train.values.ravel())

    print("Based on the training data (80% of the 500 records), I have found that the model is: ")
    print("===============================================================")
    print('Intercept: ', regr.intercept_[0])
    for i, coef in enumerate(regr.coef_[0]):
        print(f'Coefficient for {col_names[i]} : ', coef)
    print("===============================================================")

    print("Based on the testing data (20% of the 500 records), I have found that: ")
    y_prediction = regr.predict(X_test)
    # predicting the accuracy score
    accuracy = accuracy_score(y_test, y_prediction) * 100
    confusion_mat = confusion_matrix(y_test, y_prediction)
    print("Accuracy is", accuracy, "%")
    print("Confusion Matrix: ")
    t = PrettyTable(["Label", "Predict_Reject", "Predict_Admit"])
    t.add_row(["Actual_Reject"] + list(confusion_mat[0]))
    t.add_row(["Actual_Admit"] + list(confusion_mat[1]))
    t.align = "c"
    print(t)
    return regr


def make_prediction(df, regr, newRecord):
    col_names = list(df.columns)
    col_names = col_names[1:-1]
    df_new_record = pd.DataFrame(newRecord, col_names).T
    newPredict = regr.predict(df_new_record)
    predicted_str = ""
    if newPredict[0] > 1:
        predicted_str = "1 ( actual value is " + str(newPredict[0]) + ")"
    elif newPredict[0] < 0:
        predicted_str = "0 ( actual value is " + str(newPredict[0]) + ")"
    else:
        predicted_str = str(newPredict[0])
    print("===============================================================")
    print("The predicted admission result is: " + predicted_str)
    print("===============================================================")


# get user input for an unknown student as a list
def getUserInput():
    isConfirmed = False
    while not isConfirmed:
        gre = int(input("Please input GRE_Score [int out of 340]: "))
        toelf = int(input("Please input TOEFL_Score [int out of 120]: "))
        university_rate = int(input("Please input University_Rating [int out of 5]: "))
        statement_of_purpose = float(input("Please input SOP strength [double out of 5]: "))
        letter_of_recommendation = float(input("Please input LOR strength [double out of 5]: "))
        cgpa = float(input("Please input cGPA [double out of 10]: "))
        research = int(input("Please input Research Experience [either 0 or 1]: "))
        print("===============================================================")
        print("Please confirm your input:")
        user_input = [gre, toelf, university_rate, statement_of_purpose, letter_of_recommendation, cgpa, research]
        names = ['GRE_Score', 'TOEFL_Score', 'University_Rating', 'SOP', 'LOR_', 'CGPA', 'Research']
        t = PrettyTable(names)
        t.add_row(user_input)
        print(t)
        print("[Press Y to confirm, any other key to re-input]: ")
        isConfirmed = input() in ["Y", "y"]
        if not isConfirmed:
            print("Please re-inter the information of the student: ")
        else:
            print("Input is confirmed, please wait while training the model...")

    return user_input


if __name__ == "__main__":
    # user_input = getUserInput()
    print(
        "====================================================================================================")
    print("Welcome to the \"Logistic Regression Section\", below is the detailed trained model (80% "
          "training data and 20% testing data)")
    df = get_dataframe_from_database()
    model = train_model(df)

    isFinished = False
    while not isFinished:
        print("[Press p to make a new prediction, type q to quit]: ", end="")
        command = str(input())
        if command == "q":
            isFinished = True
        else:
            user_input = getUserInput()
            make_prediction(df, model, user_input)
    # user_input = [340, 120, 4, 4.5, 4.0, 9.92, 1]
    # make_prediction(df, model, user_input)
