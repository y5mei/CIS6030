import psycopg2
from psycopg2.extensions import ISOLATION_LEVEL_AUTOCOMMIT
import pandas as pd
from sqlalchemy import create_engine

USER_NAME = "cis6030"
USER_PASSWORD = "cis6030"
DATABASE_NAME = "postgres"
TABLE_NAME = "admission"
csv_file_name = "Admission_Predict_Ver1.1.csv"

# read the csv file to a pandas df
def save_csv_to_database():
    df = pd.read_csv(csv_file_name, skipfooter=3, engine='python')
    df.columns = df.columns.str.replace(' ', '_')
    engine = create_engine(f"postgresql://{USER_NAME}:{USER_PASSWORD}@localhost:5432/{DATABASE_NAME}")
    df.to_sql(
        TABLE_NAME,
        engine,
        index=False
    )


if __name__ == "__main__":
    save_csv_to_database()