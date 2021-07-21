import asyncio
import websockets

async def hello():
    uri = "ws://localhost:" + input()
    async with websockets.connect(uri, extra_headers={"Sec-WebSocket-Protocol":"dpl"}) as websocket:
        name = input("What's your name? ")
        
        await websocket.send(name)
        print(f"> {name}")

        greeting = await websocket.recv()
        print(f"< {greeting}")

asyncio.get_event_loop().run_until_complete(hello())