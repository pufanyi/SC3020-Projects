import plotly.graph_objs as go
from igraph import Graph
from sc3020.database.ExecutionTree import ExecutionTree


class Visualizer(object):
    def calc_layout(self, tree: ExecutionTree):
        tree.finalize_id()
        nodes = tree.dfs()
        parents = [node.parent.id for node in nodes if node.parent]
        edges = [(node.id, parent) for node, parent in zip(nodes[1:], parents)]
        g = Graph(n=len(nodes), edges=edges, directed=True)
        node_layout = g.layout("rt", root=[0], mode="all")
        min_y = min([pos[1] for pos in node_layout])
        node_layout = [(pos[0], pos[1] - min_y) for pos in node_layout]
        max_y = max([pos[1] for pos in node_layout])
        node_layout = [(pos[0], max_y - pos[1]) for pos in node_layout]
        return nodes, node_layout, edges

    def visualize(self, tree: ExecutionTree) -> go.Figure:
        fig = go.Figure()
        nodes, node_layout, edges = self.calc_layout(tree)
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
        # markers = [node.get_marker() for node in nodes]
        fig.add_trace(
            go.Scatter(
                x=[pos[0] for pos in node_layout],
                y=[pos[1] for pos in node_layout],
                mode="markers+text",
                showlegend=False,
                marker=dict(
                    symbol=[node.symbol() for node in nodes],
                    size=35,
                    opacity=1,
                    color="#6175c1",
                    line=dict(color="rgb(50,50,50)", width=1),
                ),
                text=[node.get_text() for node in nodes],
                textposition="middle center",
                textfont=dict(size=14, color="white"),
                hoverinfo="text",
                hovertext=[node.explain() for node in nodes],
                hoverlabel=dict(font=dict(family="monospace")),
            )
        )
        layer = max([pos[1] for pos in node_layout]) + 1
        height = max(300, 55 * layer)
        fig.update_layout(
            showlegend=False,
            margin=dict(l=0, r=0, t=0, b=0),
            plot_bgcolor="rgba(0,0,0,0)",
            paper_bgcolor="rgba(0,0,0,0)",
            xaxis=dict(showgrid=False, zeroline=False, showticklabels=False),
            yaxis=dict(showgrid=False, zeroline=False, showticklabels=False),
            height=height,
        )
        return fig
