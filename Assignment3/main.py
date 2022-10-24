import psycopg2
from psycopg2.extensions import ISOLATION_LEVEL_AUTOCOMMIT
import pandas as pd
from sqlalchemy import create_engine
from YourConstant import *


# read the csv file to a pandas df
def save_csv_to_database():
    try:
        df = pd.read_csv(csv_file_name, skipfooter=3, engine='python')
        df.columns = df.columns.str.replace(' ', '_')
        engine = create_engine(f"postgresql://{USER_NAME}:{USER_PASSWORD}@localhost:5432/{DATABASE_NAME}")
        df.to_sql(
            TABLE_NAME,
            engine,
            index=False
        )
        print("============================================================================================")
        print(f"The Creation of Table {TABLE_NAME} is Done. You can perform Linear/Logistic Regression now!")
        print("============================================================================================")
    except Exception as error:
        print("============================================================================================")
        print(f"The Creation of Table {TABLE_NAME} Failed for the following reason: ")
        print(error)
        print("============================================================================================")


if __name__ == "__main__":
    save_csv_to_database()
