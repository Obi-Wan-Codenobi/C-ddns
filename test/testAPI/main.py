from fastapi import FastAPI, HTTPException, Request
from fastapi.middleware.cors import CORSMiddleware
from fastapi.routing import APIRouter
import json
import os

app = FastAPI()

api_router = APIRouter()

# Enable CORS for your FastAPI application
app.add_middleware(
    CORSMiddleware,
    allow_origins=["*"],  # List of origins that are allowed to make requests
    allow_credentials=True,
    allow_methods=["*"],  # List of HTTP methods that are allowed
    allow_headers=["*"],  # List of HTTP headers that are allowed
)

@api_router.get("/")
def root():
    return {"Hello": "World"}

@api_router.post("/")
async def return_msg(request: Request):
    try:
        data = await request.json()
        print(data)
        return {"status": "success", "message": "Data written to file"}
    except Exception as e:
        raise HTTPException(status_code=500, detail=str(e))


app.include_router(api_router)

