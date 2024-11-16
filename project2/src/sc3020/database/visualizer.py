import io

import plotly.graph_objects as go
from igraph import EdgeSeq, Graph
from sc3020.database.ExecutionTree import ExecutionTree, ExecutionTreeNode


class Visualizer(object):
    def calc_layout(self, tree: ExecutionTree):
        # Create a dictionary mapping node IDs to ExecutionTreeNodes
        tree.finalize_id()
        nodes = tree.dfs()
        # Create vertex list from 0 to len(nodes)-1
        parents = [node.parent.id for node in nodes if node.parent]
        edges = [(node.id, parent) for node, parent in zip(nodes[1:], parents)]
        g = Graph(n=len(nodes), edges=edges, directed=True)
        # Increase the depth of the tree layout by setting a larger height
        node_layout = g.layout("rt", root=[0], mode="all")
        print(list(node_layout))
        return nodes, node_layout, edges

    def draw(self, tree: ExecutionTree) -> go.Figure:
        fig = go.Figure()
        nodes, node_layout, edges = self.calc_layout(tree)
        fig.add_trace(
            go.Scatter(
                x=[pos[0] for pos in node_layout],
                y=[pos[1] for pos in node_layout],
                mode="markers",
                name="bla",
                marker=dict(
                    symbol="circle-dot",
                    size=18,
                    color="#6175c1",  # '#DB4551',
                    line=dict(color="rgb(50,50,50)", width=1),
                ),
                hoverinfo="text",
                text=[node.id for node in nodes],
                opacity=0.8,
            )
        )
        Xe, Ye = [], []
        for edge in edges:
            print(edge[0], edge[1])
            Xe += [node_layout[edge[0]][0], node_layout[edge[1]][0], None]
            Ye += [node_layout[edge[0]][1], node_layout[edge[1]][1], None]
        fig.add_trace(
            go.Scatter(
                x=Xe,
                y=Ye,
                mode="lines",
                line=dict(color="rgb(210,210,210)", width=1),
                hoverinfo="none",
            )
        )
        return fig

    def visualize(self, tree: ExecutionTree) -> str:  # html
        fig = self.draw(tree)
        fig.write_html("visualizer.html", include_plotlyjs="cdn")
        return "visualizer.html"
