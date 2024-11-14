import argparse
import subprocess
import sys

import psycopg


def parse_argument():
    parser = argparse.ArgumentParser()
    parser.add_argument("--host", type=str, default="localhost", help="host name")
    parser.add_argument("--port", type=int, default=5432, help="port number")
    parser.add_argument("--user", type=str, default="sc3020", help="user name")
    parser.add_argument("--password", type=str, default="sc3020", help="password")
    parser.add_argument("--database", type=str, default="mydb", help="database name")
    parser.add_argument(
        "--tbl-dir",
        type=str,
        default="../sc3020/assets/data",
        help="directory of tables",
    )
    args = parser.parse_args()
    return args


if __name__ == "__main__":
    args = parse_argument()
    host: str = args.host
    port: int = args.port
    user: str = args.user
    password: str = args.password
    database: str = args.database
    connection_string: str = (
        f"host={host} port={port} user={user} password={password} dbname={database}"
    )
    with psycopg.connect(conninfo=connection_string) as conn:
        with conn.cursor() as cur:
            # supplier
            cur.execute(
                """
                        CREATE TABLE IF NOT EXISTS "supplier" (
                        "s_suppkey"     INT,
                        "s_name"        CHAR(25),
                        "s_address"     VARCHAR(40),
                        "s_nationkey"   INT,
                        "s_phone"       CHAR(15),
                        "s_acctbal"     DECIMAL(15,2),
                        "s_comment"     VARCHAR(101),
                        "s_dummy"       VARCHAR(10),
                        PRIMARY KEY ("s_suppkey"));
                        """
            )

            # customer
            cur.execute(
                """
                        CREATE TABLE IF NOT EXISTS "customer" (
                        "c_custkey"     INT,
                        "c_name"        VARCHAR(25),
                        "c_address"     VARCHAR(40),
                        "c_nationkey"   INT,
                        "c_phone"       CHAR(15),
                        "c_acctbal"     DECIMAL(15,2),
                        "c_mktsegment"  CHAR(10),
                        "c_comment"     VARCHAR(117),
                        "c_dummy"       VARCHAR(10),
                        PRIMARY KEY ("c_custkey"));
                        """
            )

            # part
            cur.execute(
                """
                        CREATE TABLE IF NOT EXISTS "part" (
                        "p_partkey"     INT,
                        "p_name"        VARCHAR(55),
                        "p_mfgr"        CHAR(25),
                        "p_brand"       CHAR(10),
                        "p_type"        VARCHAR(25),
                        "p_size"        INT,
                        "p_container"   CHAR(10),
                        "p_retailprice" DECIMAL(15,2) ,
                        "p_comment"     VARCHAR(23) ,
                        "p_dummy"       VARCHAR(10),
                        PRIMARY KEY ("p_partkey"));
                        """
            )

            # partsupp
            cur.execute(
                """
                        CREATE TABLE IF NOT EXISTS "partsupp" (
                        "ps_partkey"     INT,
                        "ps_suppkey"     INT,
                        "ps_availqty"    INT,
                        "ps_supplycost"  DECIMAL(15,2),
                        "ps_comment"     VARCHAR(199),
                        "ps_dummy"       VARCHAR(10),
                        PRIMARY KEY ("ps_partkey"));
                        """
            )

            # orders
            cur.execute(
                """
                        CREATE TABLE IF NOT EXISTS "orders" (
                        "o_orderkey"       INT,
                        "o_custkey"        INT,
                        "o_orderstatus"    CHAR(1),
                        "o_totalprice"     DECIMAL(15,2),
                        "o_orderdate"      DATE,
                        "o_orderpriority"  CHAR(15),
                        "o_clerk"          CHAR(15),
                        "o_shippriority"   INT,
                        "o_comment"        VARCHAR(79),
                        "o_dummy"          VARCHAR(10),
                        PRIMARY KEY ("o_orderkey"));
                        """
            )

            # lineitem
            cur.execute(
                """
                        CREATE TABLE IF NOT EXISTS "lineitem"(
                        "l_orderkey"          INT,
                        "l_partkey"           INT,
                        "l_suppkey"           INT,
                        "l_linenumber"        INT,
                        "l_quantity"          DECIMAL(15,2),
                        "l_extendedprice"     DECIMAL(15,2),
                        "l_discount"          DECIMAL(15,2),
                        "l_tax"               DECIMAL(15,2),
                        "l_returnflag"        CHAR(1),
                        "l_linestatus"        CHAR(1),
                        "l_shipdate"          DATE,
                        "l_commitdate"        DATE,
                        "l_receiptdate"       DATE,
                        "l_shipinstruct"      CHAR(25),
                        "l_shipmode"          CHAR(10),
                        "l_comment"           VARCHAR(44),
                        "l_dummy"             VARCHAR(10));
                        """
            )

            cur.execute(
                f"""
                        COPY supplier
                        FROM '{args.tbl_dir}/supplier.tbl'
                        DELIMITER '|'
                        CSV;
                        """
            )

            cur.execute(
                f"""
                        COPY customer
                        FROM '{args.tbl_dir}/customer.tbl'
                        DELIMITER '|'
                        CSV;
                        """
            )

            cur.execute(
                f"""
                        COPY part
                        FROM '{args.tbl_dir}/part.tbl'
                        DELIMITER '|'
                        CSV;
                        """
            )

            # cur.execute(f"""
            # COPY partsupp
            # FROM '{args.tbl_dir}/partsupp.tbl'
            # DELIMITER '|'
            # CSV;
            # """)

            cur.execute(
                f"""
                        COPY orders
                        FROM '{args.tbl_dir}/orders.tbl'
                        DELIMITER '|'
                        CSV;
                        """
            )

            cur.execute(
                f"""
                        COPY lineitem
                        FROM '{args.tbl_dir}/lineitem.tbl'
                        DELIMITER '|'
                        CSV;
                        """
            )

            conn.commit()
    # res = subprocess.run(f'psql -c "\copy "customer"     from \'{args.tbl_dir}/customer.tbl\'        DELIMITER \'|\' CSV;" {user} {database}', stdout=subprocess.PIPE)
    # print(res.stdout.decode(sys.stdout.encoding))
