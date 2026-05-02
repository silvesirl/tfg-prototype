import requests
import time
import random

URL_BASE = "http://localhost:18080"
NumberOfRequests = 50
Latencies = []

def TestEndpoint(aName, aPath, aPayload=None):
    print(f"--- Testing: {aName} ---")
    StartRequest = time.perf_counter()
    
    if aPayload:
        Response = requests.post(f"{URL_BASE}{aPath}", json=aPayload)
    else:
        Response = requests.post(f"{URL_BASE}{aPath}")
        
    EndRequest = time.perf_counter()
    Latency = EndRequest - StartRequest
    
    if Response.status_code == 200:
        print(f"Success: {aName} | Time: {Latency:.4f}s")
        return Response.json()
    else:
        print(f"Error in {aName}: Status {Response.status_code}")
        return None

TestEndpoint("Change Metric to KM", "/changekm")
TestEndpoint("Change Metric to Miles", "/changemiles")
TestEndpoint("Change Metric to Feet", "/changefoot")

PayloadContinent = {"continent": "Europe"}
TestEndpoint("Update Continent Filter", "/changecontinent", PayloadContinent)

PayloadCategory = {"category": "Monumento"}
TestEndpoint("Update Category Filter", "/changecategory", PayloadCategory)

PayloadLanguage = {"language": "ch"}
TestEndpoint("Update Language", "/changelanguage", PayloadLanguage)

print(f"\n--- Starting Stress Test: /calculate ({NumberOfRequests} requests) ---")
StartTotal = time.perf_counter()

for i in range(NumberOfRequests):
    PayloadCalculate = {
        "lat": random.uniform(-90, 90),
        "lon": random.uniform(-180, 180)
    }
    
    StartReq = time.perf_counter()
    Response = requests.post(f"{URL_BASE}/calculate", json=PayloadCalculate)
    EndReq = time.perf_counter()
    
    Latencies.append(EndReq - StartReq)

EndTotal = time.perf_counter()

print(f"Total time for {NumberOfRequests} requests: {EndTotal - StartTotal:.4f}s")
print(f"Average latency per request: {sum(Latencies)/len(Latencies):.4f}s")