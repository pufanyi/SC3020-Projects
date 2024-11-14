import os

import gradio as gr
import psycopg
from fastapi import FastAPI, Request
from fastapi.responses import HTMLResponse
from fastapi.staticfiles import StaticFiles
from fastapi.templating import Jinja2Templates

os.chdir(os.path.dirname(os.path.abspath(__file__)))

app = FastAPI()

app.mount("/static", StaticFiles(directory="static"), name="static")
app.mount("/assets", StaticFiles(directory="assets"), name="assets")

CUSTOM_PATH = "/gradio"
templates = Jinja2Templates(directory="templates")


@app.get("/", response_class=HTMLResponse)
async def main(request: Request):
    return templates.TemplateResponse("index.html", {"request": request})


with gr.Blocks() as demo:
    gr.Markdown("## Connect to TPC-H Database")
    with gr.Row(equal_height=True) as row:
        url = gr.Textbox(lines=1, label="URL", value="localhost")
        port = gr.Number(label="Port", value=5432)
        username = gr.Textbox(lines=1, label="Username", value="postgres")
        password = gr.Textbox(label="Password", type="password")
        database = gr.Textbox(lines=1, label="Database", value="tpch")

app = gr.mount_gradio_app(app, demo, path=CUSTOM_PATH)
