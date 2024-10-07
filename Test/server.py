from phew import server, connect_to_wifi

connect_to_wifi("Kavin", "12345678")

@server.route("/random", methods=["GET"])
def random_number(request):
  import random
  min = int(request.query.get("min", 0))
  max = int(request.query.get("max", 100))
  return str(random.randint(min, max))

@server.catchall()
def catchall(request):
  return "Not found", 404

server.run()
