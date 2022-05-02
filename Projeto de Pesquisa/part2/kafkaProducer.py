from kafka import KafkaProducer
import sys

if __name__ == '__main__':
  if len(sys.argv) != 3:
    print('Usage: kafkaProducer.py <hostname>:<port> <topic>', file=sys.stderr)
  
  producer = KafkaProducer(bootstrap_servers=sys.argv[1])

  while True:
    message = input()
    producer.send(sys.argv[2], str.encode(message))