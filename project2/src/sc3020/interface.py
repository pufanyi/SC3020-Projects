import os
from pathlib import Path
from typing import Union

import gradio as gr
import sc3020.database.tcph as tcph
from fastapi import FastAPI, Request
from fastapi.responses import HTMLResponse
from fastapi.staticfiles import StaticFiles
from fastapi.templating import Jinja2Templates
from sc3020.whatif import JOIN_REGISTRY, SCAN_REGISTRY

os.chdir(os.path.dirname(os.path.abspath(__file__)))

app = FastAPI()

app.mount("/static", StaticFiles(directory="static"), name="static")
app.mount("/assets", StaticFiles(directory="assets"), name="assets")

CUSTOM_PATH = "/sc3020"
templates = Jinja2Templates(directory="templates")

TEMPLATE_PATH = Path(__file__).parent / "templates"
EXAMPLE_PATH = Path(__file__).parent / "examples"


@app.get("/", response_class=HTMLResponse)
async def main(request: Request):
    return templates.TemplateResponse("index.html", {"request": request})


def connect_db(db: tcph.TPCHDataset):
    with gr.Accordion("Connect to Database", open=True):
        with open(TEMPLATE_PATH / "connection_guide.md", "r") as f:
            connection_guide = f.read()

        gr.Markdown(connection_guide)

        with gr.Row(equal_height=True):
            db_info = [
                gr.Textbox(lines=1, label="Host", value="localhost", interactive=True),
                gr.Number(label="Port", value=5432, interactive=True),
                gr.Textbox(lines=1, label="Database", value="tpch", interactive=True),
                gr.Textbox(
                    lines=1, label="Username", value="postgres", interactive=True
                ),
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
    with gr.Accordion("Query Console", open=True):

        def save_fig(
            query_input: str, format: str = "html", path: Union[str, Path, None] = None
        ):
            if query_input is None:
                return
            if path is None:
                path = Path(__file__).parent / "assets" / "cache"
            _, _, _, fig = db.explain(query_input)
            path.mkdir(exist_ok=True, parents=True)
            if isinstance(path, str):
                path = Path(path)
            if not path.is_file():
                path = path / f"query_plan.{format}"
            try:
                if format == "html":
                    fig.write_html(path, include_mathjax="cdn")
                elif format == "json":
                    fig.write_json(path)
                else:
                    fig.write_image(path, format=format)
                return path
            except Exception as e:
                gr.Error(f"Failed to save query plan: {str(e)}")

        examples = list(EXAMPLE_PATH.glob("example*.sql"))

        with gr.Row(equal_height=True):
            query_input = gr.Code(
                lines=1,
                label="Query",
                interactive=True,
                language="sql-pgSQL",
            )
            with gr.Column():
                query_plan_fig = gr.Plot(label="Query Plan")
                with gr.Column():
                    save_format = gr.Dropdown(
                        choices=["svg", "html", "pdf", "png", "jpeg", "json"],
                        label="Format",
                        value="svg",
                        show_label=False,
                        container=False,
                    )
                    gr.DownloadButton(
                        "Save", value=save_fig, inputs=[query_input, save_format]
                    )

        with gr.Row():
            query_btns = {}
            for id, _ in enumerate(examples, 1):
                query_btns[id] = gr.Button(f"Example {id}")

        with gr.Row():
            estimate_startup_cost = gr.Number(
                label="Estimate Startup Cost", precision=2
            )
            estimate_total_cost = gr.Number(label="Estimate Total Cost", precision=2)

        with gr.Row():
            scan_dropdown = gr.Dropdown(
                choices=[k for k in SCAN_REGISTRY.keys()] + ["Default"],
                label="What if change scan to ...",
                value="Default",
            )
            join_dropdown = gr.Dropdown(
                choices=[k for k in JOIN_REGISTRY.keys()] + ["Default"],
                label="What if change join to ...",
                value="Default",
            )

        with gr.Row():
            with gr.Accordion("Explain", open=False):
                explain = gr.Markdown()

        with gr.Row():
            refresh_estimation = gr.Button("Refresh Estimation", visible=True)
            query_btn = gr.Button("Execute", visible=True)
            # whatif_btn = gr.Button("Execute with What If...", visible=True)

        with gr.Row():
            result = gr.DataFrame(value=[], label="Result (Top 100 rows)")

        with gr.Row():
            query_logs = gr.JSON({}, label="Logs")

        def make_click_fn(query_text: str):
            def click_handler(query_input: str):
                if query_input == query_text:
                    return query_text, *db.explain(query_text)
                else:
                    return query_text, None, None, None, None

            return click_handler

        for id, example in enumerate(examples, 1):
            with open(example, "r") as f:
                query = f.read()
            query_btns[id].click(
                fn=make_click_fn(query),
                inputs=[query_input],
                outputs=[
                    query_input,
                    explain,
                    estimate_total_cost,
                    estimate_startup_cost,
                    query_plan_fig,
                ],
            )

        refresh_estimation.click(
            fn=db.explain,
            inputs=[query_input],
            outputs=[
                explain,
                estimate_total_cost,
                estimate_startup_cost,
                query_plan_fig,
            ],
        )

        query_input.change(
            fn=db.explain,
            outputs=[
                explain,
                estimate_total_cost,
                estimate_startup_cost,
                query_plan_fig,
            ],
            inputs=[query_input],
        )

        query_btn.click(
            fn=db.execute,
            inputs=[query_input],
            outputs=[
                result,
                query_logs,
            ],
        )
        scan_dropdown.change(
            fn=db.explain_with_what_if,
            inputs=[query_input, scan_dropdown, join_dropdown],
            outputs=[
                explain,
                estimate_total_cost,
                estimate_startup_cost,
                query_plan_fig,
            ],
        )
        join_dropdown.change(
            fn=db.explain_with_what_if,
            inputs=[query_input, scan_dropdown, join_dropdown],
            outputs=[
                explain,
                estimate_total_cost,
                estimate_startup_cost,
                query_plan_fig,
            ],
        )


def db_overview(
    dataset: str = "pufanyi/TPC-H", split: str = "train", default_subset="customer"
):
    with gr.Accordion("Database Overview", open=False):
        gr.HTML(
            f"""<iframe
                src="https://huggingface.co/datasets/{dataset}/embed/viewer/{default_subset}/{split}"
                frameborder="0"
                width="100%"
                height="560px"
                ></iframe>"""
        )


with open(TEMPLATE_PATH / "header.html", "r") as f:
    header = f.read()

with open(TEMPLATE_PATH / "title.html", "r") as f:
    title = f.read()

with gr.Blocks(head=header, theme=gr.themes.Default(text_size="lg")) as demo:
    gr.HTML(title)

    db = tcph.TPCHDataset()
    db = connect_db(db)

    db_overview()

    query_console(db)

app = gr.mount_gradio_app(app, demo, path=CUSTOM_PATH)
