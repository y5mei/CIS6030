import psycopg2
from psycopg2.extensions import ISOLATION_LEVEL_AUTOCOMMIT
import pandas as pd
from sqlalchemy import create_engine

# How to hide the credentials???
# conn = psycopg2.connect("host=localhost dbname=postgres user=cis6030 password=cis6030")
# cur = conn.cursor()
# cur.execute("""
#     CREATE TABLE admission(
#     SerialNum integer PRIMARY KEY,
#     GRE integer,
#     TOEFL integer,
#     UNIVERSITY integer,
#     SOP double precision,
#     LOR double precision,
#     CGPA double precision,
#     Research integer,
#     Chance double precision
# )""")
# conn.commit()
USER_NAME = "cis6030"
USER_PASSWORD = "cis6030"
DATABASE_NAME = "postgres"
TABLE_NAME = "admission"

# con = psycopg2.connect(f"host=localhost dbname=postgres user={USER_NAME} password={USER_PASSWORD}")
# con.set_isolation_level(ISOLATION_LEVEL_AUTOCOMMIT)
# cursor = con.cursor()
# sqlCreateDatabase = "CREATE DATABASE " + DATABASE_NAME
# cursor.execute(sqlCreateDatabase)

# read the csv file to a pandas df

csv_file_name = "Admission_Predict_Ver1.1.csv"
df = pd.read_csv(csv_file_name, skipfooter=3, engine='python')
df.columns = df.columns.str.replace(' ', '_')
print(df)
engine = create_engine(f"postgresql://{USER_NAME}:{USER_PASSWORD}@localhost:5432/{DATABASE_NAME}")
df.to_sql(
    TABLE_NAME,
    engine,
    index=False
)
