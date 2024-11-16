import io

import plotly.graph_objects as go
from igraph import EdgeSeq, Graph
from sc3020.database.ExecutionTree import ExecutionTree, ExecutionTreeNode


class Visualizer(object):
    def calc_layout(self, tree: ExecutionTree):
        tree.finalize_id()
        nodes = tree.dfs()
        parents = [node.parent.id for node in nodes if node.parent]
        edges = [(node.id, parent) for node, parent in zip(nodes[1:], parents)]
        g = Graph(n=len(nodes), edges=edges, directed=True)
        node_layout = g.layout("rt", root=[0], mode="all")
        max_y = max([pos[1] for pos in node_layout])
        node_layout = [(pos[0], max_y - pos[1]) for pos in node_layout]
        return nodes, node_layout, edges

    def visualize(self, tree: ExecutionTree) -> go.Figure:
        fig = go.Figure()
        nodes, node_layout, edges = self.calc_layout(tree)
        fig.add_trace(
            go.Scatter(
                x=[pos[0] for pos in node_layout],
                y=[pos[1] for pos in node_layout],
                mode="markers",
                showlegend=False,
                marker=dict(
                    symbol="circle-dot",
                    size=18,
                    color="#6175c1",
                    line=dict(color="rgb(50,50,50)", width=1),
                ),
                hoverinfo="text",
                text=[node.id for node in nodes],
                opacity=0.8,
            )
        )
        Xe, Ye = [], []
        for edge in edges:
            Xe += [node_layout[edge[0]][0], node_layout[edge[1]][0], None]
            Ye += [node_layout[edge[0]][1], node_layout[edge[1]][1], None]
        fig.add_trace(
            go.Scatter(
                x=Xe,
                y=Ye,
                mode="lines",
                showlegend=False,
                line=dict(color="rgb(210,210,210)", width=1),
                hoverinfo="none",
            )
        )
        fig.update_layout(
            showlegend=False,
            margin=dict(l=0, r=0, t=0, b=0),
            plot_bgcolor="rgba(0,0,0,0)",
            paper_bgcolor="rgba(0,0,0,0)",
            xaxis=dict(showgrid=False, zeroline=False, showticklabels=False),
            yaxis=dict(showgrid=False, zeroline=False, showticklabels=False),
        )
        return fig
