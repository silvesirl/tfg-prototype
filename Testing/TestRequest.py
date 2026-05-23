import requests
import time
import random

URL_BASE = "http://localhost:18080"
NumberOfRequests = 500

session = requests.Session()

def TestEndpoint(aName, aPath, aPayload=None):
    print(f"--- Testing: {aName} ---")
    StartRequest = time.perf_counter()
    
    if aPayload:
        Response = session.post(f"{URL_BASE}{aPath}", json=aPayload)
    else:
        Response = session.post(f"{URL_BASE}{aPath}")
        
    EndRequest = time.perf_counter()
    Latency = EndRequest - StartRequest
    
    if Response.status_code == 200:
        print(f"Success: {aName} | Latency: {Latency:.4f}s")
        try:
            return Response.json()
        except ValueError:
            return Response.text
    else:
        print(f"Error in {aName}: Status {Response.status_code} | Time: {Latency:.4f}s")
        return None

print("=== WARMING UP SERVER SOCKETS ===")
try:
    session.post(f"{URL_BASE}/changekm") 
except Exception:
    pass
print("Server connection established. Starting clean benchmark...\n")

print("=== STARTING CONFIGURATION ENDPOINTS TEST ===")
TestEndpoint("Change Metric to KM", "/changekm")
TestEndpoint("Change Metric to Miles", "/changemiles")
TestEndpoint("Change Metric to Feet", "/changefoot")

PayloadContinent = {"continent": "Europe"}
TestEndpoint("Update Continent Filter", "/changecontinent", PayloadContinent)

PayloadType = {"type": "monument"}
TestEndpoint("Update Type Filter", "/changecategory", PayloadType)

PayloadLanguage = {"language": "ch"}
TestEndpoint("Update Language", "/changelanguage", PayloadLanguage)


print(f"\n=== STARTING STRESS TEST: /calculate ({NumberOfRequests} requests) ===")
CalculateLatencies = []
StartTotal = time.perf_counter()

for i in range(NumberOfRequests):
    PayloadCalculate = {
        "lat": random.uniform(-90, 90),
        "lon": random.uniform(-180, 180)
    }
    
    StartReq = time.perf_counter()
    Response = session.post(f"{URL_BASE}/calculate", json=PayloadCalculate)
    EndReq = time.perf_counter()
    
    CalculateLatencies.append(EndReq - StartReq)

EndTotal = time.perf_counter()

print("\n=== FINAL BENCHMARK REPORT ===")
print(f"Total time for {NumberOfRequests} stress requests: {EndTotal - StartTotal:.4f}s")
print(f"Average latency per /calculate request: {sum(CalculateLatencies)/len(CalculateLatencies):.4f}s")