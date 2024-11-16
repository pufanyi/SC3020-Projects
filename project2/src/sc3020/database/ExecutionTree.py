from typing import List, Tuple


class ExecutionTreeNode:
    def __init__(self):
        self.children: List[ExecutionTreeNode] = []
        self.parent = None
        self.condition: List[str] = []
        self.operation: str = None

    def add_child(self, child):
        self.children.append(child)

    def set_level(self, level: int):
        self.level = level

    def set_parent(self, parent):
        self.parent = parent

    def set_condition(self, condition: List[str]):
        self.condition = condition

    def add_condition(self, condition: str):
        self.condition.append(condition.strip())

    def set_operation(self, operation: str):
        self.operation, self.info = operation.split("  ")
        self.parse_info(self.info)

    def parse_info(self, info: str):
        # Info looks like this
        # (cost=0.00..0.00 rows=0 width=0)
        # for cost, first is the startup cost, second is the total cost
        # for rows, it is the estimated number of rows output
        # for width, it is estimated average width of rows output
        info = info.replace("(", "").replace(")", "")
        cost, rows, width = info.split(" ")
        self.startup_cost = float(cost.split("..")[0].split("=")[1])
        self.total_cost = float(cost.split("..")[1])
        self.rows = float(rows.split("=")[1])
        self.width = float(width.split("=")[1])

    def __repr__(self):
        operation = self.operation.split("  ")[0].strip()
        condition = self.condition
        level = self.level
        if len(condition) > 0:
            return f"{' ' * level * 2}{operation} with Cond : {' '.join(condition)}"
        else:
            return f"{' ' * level * 2}{operation}"

    def natural_language(self):
        operation = self.operation
        condition = self.condition
        level = self.level
        natural_language_str = f"Perform {operation}"
        for cond in condition:
            total_split = cond.split(":")
            key = total_split[0].strip()
            value = ":".join(total_split[1:])
            natural_language_str += f" with condition {key} on {value}"

        return natural_language_str


class ExecutionTree:
    def __init__(self):
        self.root: ExecutionTreeNode = None

    def set_root(self, root: ExecutionTreeNode):
        self.root = root

    def traversal(self):
        return self._traversal(self.root)

    def _traversal(self, node: ExecutionTreeNode) -> List[ExecutionTreeNode]:
        result = []
        for child in node.children:
            result.extend(self._traversal(child))
        # Traverse child then parent
        result.append(node)
        return result

    def get_cost(self):
        nodes = self.traversal()
        total_cost = 0
        startup_cost = 0
        for node in nodes:
            total_cost += node.total_cost
            startup_cost += node.startup_cost
        return total_cost, startup_cost


def parse_query_explanation_to_tree(explanation: List[Tuple[str]]) -> ExecutionTree:
    tree = ExecutionTree()
    root = ExecutionTreeNode()
    root.set_level(0)
    tree.set_root(root)
    current_node = root
    # Store all nodes we added so far
    all_nodes = [root]
    # The level of the node is determined by the arrow position
    arrow_places = [0]
    current_node.set_operation(explanation[0][0])
    current_level = 0
    # The first line must be the root node
    for idx, (query_plan) in enumerate(explanation[1:]):
        query_plan = query_plan[0]  # The query plan is a tuple
        if is_cond(query_plan):
            current_node.add_condition(query_plan)
        elif is_query(query_plan):
            new_node = ExecutionTreeNode()
            arrow_position = query_plan.index("->")
            if arrow_position not in arrow_places:
                arrow_places.append(arrow_position)
            level = arrow_places.index(arrow_position)

            # If we access to a child node of current node
            if level > current_level:
                # The index of the arrow index is the level of the node
                new_node.set_level(level)
                # Add operation
                new_node.set_operation(query_plan.split("->")[-1].strip())
                # Set parents to current node
                new_node.set_parent(current_node)
                # Append this node
                all_nodes.append(new_node)
                # Add the child to the current node
                current_node.add_child(new_node)
                current_node = new_node
                # Set to next level
                current_level = level
            elif level <= current_level:
                # If this level is less than the current level
                # We pop the last node util we find the closest
                while all_nodes[-1].level >= level:
                    all_nodes.pop()

                new_node.set_level(level)
                new_node.set_operation(query_plan.split("->")[-1].strip())
                new_node.set_parent(all_nodes[-1].parent)
                all_nodes[-1].parent.add_child(new_node)
                all_nodes.append(new_node)
                current_level = level
                current_node = new_node
                # import pdb; pdb.set_trace()

    return tree


def is_query(plan: str) -> bool:
    return "->" in plan


def is_cond(plan: str) -> bool:
    return ":" in plan
