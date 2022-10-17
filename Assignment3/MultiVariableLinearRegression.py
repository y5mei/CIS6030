import psycopg2
from psycopg2.extensions import ISOLATION_LEVEL_AUTOCOMMIT
import pandas as pd
from sklearn import linear_model
import matplotlib.pyplot as plt
from sklearn.model_selection import train_test_split
from sklearn.metrics import r2_score
from sklearn.metrics import mean_squared_error
from sklearn.model_selection import cross_val_predict
from prettytable import PrettyTable


# get data from database as pandas df
def get_dataframe_from_database():
    # read all the data from the database to build linear regression
    USER_NAME = "cis6030"
    USER_PASSWORD = "cis6030"
    DATABASE_NAME = "postgres"
    TABLE_NAME = "admission"

    con = psycopg2.connect(f"host=localhost dbname=postgres user={USER_NAME} password={USER_PASSWORD}")
    cursor = con.cursor()
    cursor.execute("""SELECT COLUMN_NAME  FROM INFORMATION_SCHEMA.COLUMNS WHERE TABLE_NAME = N'admission'""")
    col_names = [_[0] for _ in cursor.fetchall()]

    cursor = con.cursor()
    cursor.execute("""SELECT * FROM admission""")
    all_records = cursor.fetchall()

    df = pd.DataFrame(all_records, columns=col_names)
    return df


# train a model from a df, print out the detail of the model and return the model out
def train_model_without_userinput(df):
    # there is no need to re-normalize the inputs for regression
    X = df.iloc[:, 1:8]
    y = df.iloc[:, 8:9]
    X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.2,
                                                        random_state=19)  # 20% data were used for testing, 80% data were used for training

    regr = linear_model.LinearRegression()
    regr.fit(X_train, y_train)
    col_names = list(df.columns)

    print("Based on the training data (80% of the 500 records), I have found that: ")
    print("===============================================================")
    print('Intercept: ', regr.intercept_[0])
    for i, coef in enumerate(regr.coef_[0]):
        print(f'Coefficient for {col_names[1 + i]} : ', coef)
    print("===============================================================")

    print("Based on the testing data (20% of the 500 records), I have found that: ")
    y_prediction = regr.predict(X_test)
    # predicting the accuracy score
    score = r2_score(y_test, y_prediction)
    error = mean_squared_error(y_test, y_prediction)
    print("R^2 score of the trained regression function is", score)
    print("Mean Squared Error is ", mean_squared_error(y_test, y_prediction))
    print("===============================================================")
    return regr
    # fig, ax = plt.subplots()
    # ax.scatter(y_test, y_prediction, edgecolors=(0, 0, 0))
    # ax.plot([y.min(), y.max()], [y.min(), y.max()], "k--", lw=4)
    # ax.set_xlabel("Actual Prob of Admission ")
    # ax.set_ylabel("Predicted Prob of Admission")
    # ax.set_title('R2: ' + f'{score:.4f}' + "\n Mean Squared Error = " + f'{error:.4f}')
    # plt.show()


# use df and the user input record to predict the admission rate

def get_cal_names_for_user_input(df):
    col_names = list(df.columns)
    col_names = col_names[1:-1]
    return col_names


def predict_a_new_student(regr, newRecord, col_names):
    df_new_record = pd.DataFrame(newRecord, col_names).T
    newPredict = regr.predict(df_new_record)
    print("===============================================================")
    print("The predicted chance of admission is: ", newPredict[0][0])
    print("===============================================================")

# def train_model(df, newRecord):
#     col_names = list(df.columns)
#     col_names = col_names[1:-1]
#     df_new_record = pd.DataFrame(newRecord, col_names).T
#     newPredict = regr.predict(df_new_record)
#     print("===============================================================")
#     print("The predicted chance of admission is: ", newPredict[0][0])


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
            print("Input is confirmed, calculating the admission rate from the trained model now: ")

    return user_input


if __name__ == "__main__":
    print(
        "====================================================================================================")
    print("Welcome to the \"Multivariable Linear Regression Section\", below is the detailed trained model (80% "
          "training data and 20% testing data)")
    df = get_dataframe_from_database()
    col_names = get_cal_names_for_user_input(df)
    model = train_model_without_userinput(df)
    isFinished = False
    while not isFinished:
        print("[Press p to make a new prediction, type q to quit]: ", end ="")
        command = str(input())
        if command == "q":
            isFinished = True
        else:
            user_input = getUserInput()
            predict_a_new_student(model, user_input, col_names)
    # user_input = [340, 120, 4, 4.5, 4.0, 9.92, 1]
    # train_model(df, user_input)
