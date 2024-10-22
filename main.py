import time
import random
from threading import Thread


class BrakeSystemMonitor:
    def __init__(self, num_wagons=20, brakes_per_wagon=3, threshold=5.0):
        self.num_wagons = num_wagons
        self.brakes_per_wagon = brakes_per_wagon
        self.threshold = threshold
        self.wagon_pressures = [
            [random.uniform(5.0, 6.0) for _ in range(self.brakes_per_wagon)]
            for _ in range(self.num_wagons)
        ]
        self.running = True

    def simulate_pressures(self):
        while self.running:
            for wagon in range(self.num_wagons):
                for brake in range(self.brakes_per_wagon):
                    # Эмулируем давление с небольшими изменениями
                    change = random.uniform(-0.5, 0.5)
                    self.wagon_pressures[wagon][brake] = max(0.0, self.wagon_pressures[wagon][brake] + change)
            time.sleep(1)  # Обновление каждые 1 секунду

    def analyze_pressures(self):
        while self.running:
            for wagon_num, wagon in enumerate(self.wagon_pressures, start=1):
                for brake_num, pressure in enumerate(wagon, start=1):
                    if pressure < self.threshold:
                        self.report_issue(wagon_num, brake_num, pressure)
            time.sleep(1)  # Анализ каждые 1 секунду

    def report_issue(self, wagon_num, brake_num, pressure):
        print(
            f"[{time.strftime('%Y-%m-%d %H:%M:%S')}] Проблема в вагоне {wagon_num},"
            f" стоп-кран {brake_num}: давление {pressure:.2f} атмосфер!")

    def start_monitoring(self):
        self.simulation_thread = Thread(target=self.simulate_pressures)
        self.analysis_thread = Thread(target=self.analyze_pressures)
        self.simulation_thread.start()
        self.analysis_thread.start()

    def stop_monitoring(self):
        self.running = False
        self.simulation_thread.join()
        self.analysis_thread.join()


if __name__ == "__main__":
    monitor = BrakeSystemMonitor(num_wagons=20, brakes_per_wagon=3, threshold=5.0)
    try:
        monitor.start_monitoring()
        while True:
            time.sleep(0.1)
    except KeyboardInterrupt:
        monitor.stop_monitoring()
        print("Мониторинг остановлен.")
