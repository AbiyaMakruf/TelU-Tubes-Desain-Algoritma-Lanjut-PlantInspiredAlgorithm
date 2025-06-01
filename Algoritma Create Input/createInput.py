import random

def generate_connected_graph(n, edge_factor=2):
    """
    Membuat graf acak terhubung dengan n node.
    edge_factor: rata-rata banyaknya edge per node (default 2, cukup untuk graf jarang).
    """
    edges = set()
    # Buat spanning tree dulu supaya pasti terhubung
    nodes = list(range(n))
    random.shuffle(nodes)
    for i in range(1, n):
        u = nodes[i]
        v = random.choice(nodes[:i])
        edges.add((min(u, v), max(u, v)))
    # Tambahkan beberapa edge random ekstra
    target_edges = min(n * edge_factor, n*(n-1)//2)
    while len(edges) < target_edges:
        u, v = random.sample(range(n), 2)
        edge = (min(u, v), max(u, v))
        if edge not in edges:
            edges.add(edge)
    return n, sorted(list(edges))

for num_nodes in range(10, 201, 10):  # 10, 20, ..., 500
    n, edge_list = generate_connected_graph(num_nodes, edge_factor=2)
    filename = f'graph_{num_nodes}.dat'
    with open(filename, 'w') as f:
        f.write(f"{n}\n")
        for u, v in edge_list:
            f.write(f"{u} {v}\n")
    print(f"{filename} dibuat dengan {len(edge_list)} edge.")
