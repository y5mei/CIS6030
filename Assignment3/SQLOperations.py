import psycopg2
from prettytable import PrettyTable
from psycopg2.extensions import ISOLATION_LEVEL_AUTOCOMMIT
import pandas as pd
from sqlalchemy import create_engine

USER_NAME = "cis6030"
USER_PASSWORD = "cis6030"
DATABASE_NAME = "postgres"
TABLE_NAME = "admission"


def print_all_col_names():
    con = psycopg2.connect(f"host=localhost dbname=postgres user={USER_NAME} password={USER_PASSWORD}")
    cursor = con.cursor()
    cursor.execute("""SELECT COLUMN_NAME  FROM INFORMATION_SCHEMA.COLUMNS WHERE TABLE_NAME = N'admission'""")
    col_names = [_[0] for _ in cursor.fetchall()]
    t = PrettyTable(col_names)
    print(t)


def select_from_where_sql_queries(input):
    con = psycopg2.connect(f"host=localhost dbname=postgres user={USER_NAME} password={USER_PASSWORD}")
    cursor = con.cursor()
    cursor.execute("""SELECT COLUMN_NAME  FROM INFORMATION_SCHEMA.COLUMNS WHERE TABLE_NAME = N'admission'""")
    col_names = [_[0] for _ in cursor.fetchall()]
    t = PrettyTable(col_names)
    cursor = con.cursor()
    # must have the triple quotation and the double quotation
    # cursor.execute(
    #     """SELECT * FROM admission
    #         WHERE "TOEFL_Score" > 105
    #      """)
    cursor.execute(input)
    queries_result = cursor.fetchall()
    for x in queries_result:
        t.add_row(x)
    print(t)


if __name__ == "__main__":
    isFinished = False

    while not isFinished:
        try:
            print(
                "====================================================================================================")
            print("Welcome to the \"Select From Where Queries Section\", below are all the available col names in your "
                  "database:")
            print_all_col_names()
            print(">> Note that you MUST have a double quotation for any name contains Capital letters.")
            print(">> Postgres is case-sensitive about object names unless they're quoted, and unquoted names in "
                  "queries are automatically converted to lowercase")
            print(">> For Example: ")
            print("        SELECT * FROM admission WHERE TOEFL_Score > 105 [this is WRONG as you are actually "
                  "querying \"toefl_score\"  which does not exists]")
            print("        The correct syntax should be: SELECT * FROM admission WHERE \"TOEFL_Score\" > 105 [with the "
                  "double quotation mark]")
            print(
                "====================================================================================================")
            print("Please import your query command (type q to quit this program): ")
            print(">> ", end="")
            user_input = str(input())
            if user_input == "q":
                isFinished = True
            else:
                select_from_where_sql_queries(user_input)
        except BaseException as err:
            print(f"Unexpected {err}, {type(err)}")