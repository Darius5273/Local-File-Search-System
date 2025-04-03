import argparse
from flask import Flask, request, jsonify
import os

app = Flask(__name__)

def search_files(query, folder):
    matches = []
    for root, _, files in os.walk(folder):
        for file in files:
            if query.lower() in file.lower():
                matches.append(os.path.join(root, file))
    return matches

@app.route("/search", methods=["POST"])
def search():
    data = request.get_json()
    query = data.get("query", "").strip()
    if not query:
        return jsonify({"error": "Query cannot be empty"}), 400

    results = search_files(query, SEARCH_FOLDER)
    return jsonify({"results": results})

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("--dir", type=str, required=True)
    parser.add_argument("--port", type=int, required=True)
    args = parser.parse_args()
    SEARCH_FOLDER = args.dir

    app.run(port=args.port, debug=True)
