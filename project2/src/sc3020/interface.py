import os

import gradio as gr
import sc3020.database.tcph as tcph
from fastapi import FastAPI, Request
from fastapi.responses import HTMLResponse
from fastapi.staticfiles import StaticFiles
from fastapi.templating import Jinja2Templates

os.chdir(os.path.dirname(os.path.abspath(__file__)))

app = FastAPI()

app.mount("/static", StaticFiles(directory="static"), name="static")
app.mount("/assets", StaticFiles(directory="assets"), name="assets")

CUSTOM_PATH = "/sc3020"
templates = Jinja2Templates(directory="templates")


@app.get("/", response_class=HTMLResponse)
async def main(request: Request):
    return templates.TemplateResponse("index.html", {"request": request})


def connect_db(db: tcph.TPCHDataset):
    with gr.Row(equal_height=True):
        db_info = [
            gr.Textbox(lines=1, label="Host", value="localhost", interactive=True),
            gr.Number(label="Port", value=5432, interactive=True),
            gr.Textbox(lines=1, label="Database", value="tpch", interactive=True),
            gr.Textbox(lines=1, label="Username", value="postgres", interactive=True),
            gr.Textbox(label="Password", type="password", interactive=True),
        ]

    with gr.Row(equal_height=True):
        connect_btn = gr.Button("Connect", visible=True)
        reload_data = gr.Button("Load Data", visible=True)

    with gr.Row():
        status = gr.JSON({"status": "Not Connected"}, label="Status")

        def host(host_url, port, dbname, user, password):
            try:
                db.host(
                    host=host_url,
                    port=port,
                    dbname=dbname,
                    user=user,
                    password=password,
                )
                return {
                    "status": "Connected",
                    "host": f"{host_url}:{port}",
                    "dbname": dbname,
                    "user": user,
                }
            except Exception as e:
                return {"status": "Not Connected", "error": str(e)}

        def setup(host_url, port, dbname, user, password):
            try:
                db.setup(
                    host=host_url,
                    port=port,
                    dbname=dbname,
                    user=user,
                    password=password,
                )
                return {
                    "status": "Connected",
                    "message": "Data loaded",
                    "host": f"{host_url}:{port}",
                    "dbname": dbname,
                    "user": user,
                }
            except Exception as e:
                return {"status": "Not Connected", "error": str(e)}

        connect_btn.click(fn=host, inputs=db_info, outputs=[status])
        reload_data.click(fn=setup, inputs=db_info, outputs=[status])

    return db


def query_console(db: tcph.TPCHDataset):
    with gr.Row(equal_height=True):
        query_input = gr.Code(
            lines=20, label="Query", interactive=True, language="sql-pgSQL"
        )
        result = gr.DataFrame(value=[], label="Result")

    with gr.Row():
        query_btn = gr.Button("Execute", visible=True)

    with gr.Row():
        query_logs = gr.JSON({}, label="Logs")

    query_btn.click(fn=db.execute, inputs=[query_input], outputs=[result, query_logs])


with gr.Blocks() as demo:
    gr.Markdown("## Connect to TPC-H Database")

    db = tcph.TPCHDataset()
    db = connect_db(db)

    query_console(db)

app = gr.mount_gradio_app(app, demo, path=CUSTOM_PATH)
