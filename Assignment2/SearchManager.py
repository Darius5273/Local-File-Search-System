import requests
import multiprocessing


CACHE = {}
WORKERS = ["http://127.0.0.1:5001", "http://127.0.0.1:5002", "http://127.0.0.1:5003"]


def search_worker(query, worker, results_queue):
    try:
        response = requests.post(f"{worker}/search", json={"query": query}, timeout=5)
        if response.status_code == 200:
            results_queue.put(response.json().get("results", []))
    except requests.RequestException:
        print(f"Worker {worker} unavailable.")


def parallel_search(query):
    results_queue = multiprocessing.Queue()
    processes = []

    for worker in WORKERS:
        process = multiprocessing.Process(target=search_worker, args=(query, worker, results_queue))
        processes.append(process)
        process.start()

    for process in processes:
        process.join()

    results = []
    while not results_queue.empty():
        results.extend(results_queue.get())

    return results


def main():

    while True:
        query = input("Enter query word(exit to quit): ").strip()
        if query.lower() == 'exit':
            break

        if query in CACHE:
            results = CACHE[query]
        else:
            results = parallel_search(query)
            CACHE[query] = results

        results.sort(key=len)
        print("Search results:")
        for result in results:
            print(result)


if __name__ == "__main__":
    main()
