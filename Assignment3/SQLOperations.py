import psycopg2
from psycopg2.extensions import ISOLATION_LEVEL_AUTOCOMMIT
import pandas as pd
from sqlalchemy import create_engine

USER_NAME = "cis6030"
USER_PASSWORD = "cis6030"
DATABASE_NAME = "postgres"
TABLE_NAME = "admission"

con = psycopg2.connect(f"host=localhost dbname=postgres user={USER_NAME} password={USER_PASSWORD}")
cursor = con.cursor()
# must have the triple quotation and the double quotation
cursor.execute(
    """SELECT * FROM admission
        WHERE "TOEFL_Score" > 105
     """)

myresult = cursor.fetchall()
for x in myresult:
    print(x)
