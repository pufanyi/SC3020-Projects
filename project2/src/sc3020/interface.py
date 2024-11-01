import os

import gradio as gr
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
    gr.Markdown("## Demo")
    with gr.Row(equal_height=True) as row:
        text_input = gr.Textbox(
            lines=5, label="Input Text", placeholder="Enter some text here"
        )
        button = gr.Button("Submit", variant="primary")

app = gr.mount_gradio_app(app, demo, path=CUSTOM_PATH)
