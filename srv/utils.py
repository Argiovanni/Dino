import can
import time
import random
import threading
from . import socketio  # Import de l'instance créée dans __init__.py


def can_reader_thread(app):
    """Boucle de lecture du bus CAN."""
    try:
        # Configuration du bus (à adapter selon les branchements)
        bus = can.interface.Bus(channel="can0", bustype="socketcan")
    except Exception as e:
        print(f"Erreur CAN: {e}")
        return

    while True:
        msg = bus.recv(1.0)
        if msg:
            print("[DEBUG] : ", msg) # for debug
            # --- Logique de parsing ---
            data_to_send = {"id": hex(msg.arbitration_id), "data": list(msg.data)}
            print(data_to_send)
            # Exemple de décodage spécifique (id à modifié en fonction des trafic qu'on veut récup)
            if msg.arbitration_id == 0x1:
                data_to_send["type"] = "Vitesse"
                data_to_send["value"] = msg.data[0]
            elif msg.arbitration_id == 0x2:
                data_to_send["type"] = "RPM"
                data_to_send["value"] = msg.data[0]
            elif msg.arbitration_id == 0x3:
                data_to_send["type"] = "Temp"
                data_to_send["value"] = msg.data[0]

            # Envoi au Dashboard via SocketIO
            socketio.emit("can_update", data_to_send)

# def can_reader_thread(app): # for simulation
#     """
#     Simule la lecture du bus CAN pour tester l'affichage.
#     Une fois le matériel branché, on remplacera la boucle de test 
#     par la vraie lecture python-can.
#     """
#     print("LOG: Le thread de lecture CAN (Simulation) a démarré.")
    
#     # Valeurs de départ pour la simulation
#     mock_data = {
#         "rpm": 800,
#         "speed": 0,
#         "temp": 20,
#         "load": 15
#     }

#     while True:
#         # --- LOGIQUE DE SIMULATION ---
#         # On fait varier les chiffres de manière réaliste
#         mock_data["rpm"] += random.randint(-50, 50)
#         mock_data["speed"] = max(0, mock_data["speed"] + random.randint(-10, 10))
#         mock_data["temp"] = min(95, mock_data["temp"] + random.uniform(0.01, 0.05))
#         mock_data["load"] = random.randint(10, 80)

#         # Limitation du RPM pour rester réaliste
#         if mock_data["rpm"] < 800: mock_data["rpm"] = 800
#         if mock_data["rpm"] > 6500: mock_data["rpm"] = 6500

#         # --- ENVOI DES DONNÉES VIA SOCKETIO ---
#         # On émet chaque donnée séparément pour correspondre à ton dashboard.html
#         socketio.emit('can_update', {'type': 'RPM', 'value': mock_data["rpm"]})
#         socketio.emit('can_update', {'type': 'Vitesse', 'value': mock_data["speed"]})
#         socketio.emit('can_update', {'type': 'Temp', 'value': round(mock_data["temp"], 1)})
#         socketio.emit('can_update', {'type': 'Load', 'value': mock_data["load"]})

#         # On attend un peu pour ne pas saturer le processeur (10 mises à jour par seconde)
#         time.sleep(0.1)


def start_can_thread(app):
    """Lance le thread en mode 'daemon' pour qu'il s'arrête avec le serveur."""
    thread = threading.Thread(target=can_reader_thread, args=(app,))
    thread.daemon = True
    thread.start()
