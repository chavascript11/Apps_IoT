from http.server import BaseHTTPRequestHandler, HTTPServer
import json

# RapidAPI
import requests
import os
from dotenv import load_dotenv

load_dotenv()

X_RAPIDAPI_KEY = os.getenv("X_RAPIDAPI_KEY")

contador = 11


class MyHTTPRequestHandler(BaseHTTPRequestHandler):

  def _set_response(self, content_type="text/plain", status_code=200):
    self.send_response(status_code)
    self.send_header("Content-type", content_type)
    self.end_headers()

  def throw_custom_error(self, message, status_code=400):
    self._set_response("application/json", status_code)
    self.wfile.write(json.dumps({"message": message}).encode())

  def do_GET(self):
    self._set_response()
    respuesta = "El valor es: " + str(contador)
    self.wfile.write(respuesta.encode())

  def do_POST(self):
    content_length = int(self.headers["Content-Length"])
    post_data = self.rfile.read(content_length)

    try:
      body_json = json.loads(post_data.decode())
    except:
      self.throw_custom_error("Invalid JSON")
      return

    global contador

    # Check if text field is present
    if (body_json.get('text') is None):
      self.throw_custom_error("Missing text field")
      return

    url = "https://google-translate1.p.rapidapi.com/language/translate/v2"
    payload = {"q": body_json["text"], "target": "es", "source": "en"}
    headers = {"content-type": "application/x-www-form-urlencoded", "Accept-Encoding": "application/gzip", "X-RapidAPI-Key": X_RAPIDAPI_KEY, "X-RapidAPI-Host": "google-translate1.p.rapidapi.com"}
    response = requests.post(url, data=payload, headers=headers)

    # Translated text
    texto_traducido = response.json()["data"]["translations"][0]["translatedText"]

    # Respond to the client
    response_data = json.dumps({"translatedText": texto_traducido})
    self._set_response("application/json")
    self.wfile.write(response_data.encode())


def run_server(server_class=HTTPServer, handler_class=MyHTTPRequestHandler, port=7800):
  server_address = ("", port)
  httpd = server_class(server_address, handler_class)
  print(f"Starting server on port {port}...")
  httpd.serve_forever()


if __name__ == "__main__":
  run_server()