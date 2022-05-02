#!/usr/bin/env node

var amqp = require('amqplib/callback_api');

function initialize_vector(vector) {
  for (let i = 0; i < 100; i++) {
    vector[i] = Math.pow(i - 10 / 2, 2);
  }
}

function calculate_positions_of_vector(vector) {
  for (let i = 0; i < 100; i++) {
    vector[i] = Math.sqrt(vector[i]);
  }
}

function client() {
  let vector = [];

  initialize_vector(vector);
  calculate_positions_of_vector(vector);

  amqp.connect('amqp://localhost', function (error0, connection) {
    if (error0) {
      throw error0;
    }
    connection.createChannel(function (error1, channel1) {
      if (error1) {
        throw error1;
      }

      var msg = vector.toString();

      channel1.assertExchange('queue1', 'fanout', {
        durable: false,
      });
      channel1.publish('queue1', '', Buffer.from(msg));
      console.log(' [x] Sent %s', msg);

      connection.createChannel(function (error2, channel2) {
        if (error2) {
          throw error2;
        }

        channel2.assertExchange('queue2', 'fanout', {
          durable: false,
        });

        channel2.assertQueue(
          '',
          {
            exclusive: true,
          },
          function (error2, q) {
            if (error2) {
              throw error2;
            }
            console.log(
              ' [*] Waiting for messages in %s. To exit press CTRL+C',
              q.queue
            );
            channel2.bindQueue(q.queue, 'queue2', '');

            channel2.consume(
              q.queue,
              function (msg) {
                console.log('Aqui')
                if (msg.content) {
                  console.log(
                    ' [x] %s',
                    msg.content.toString().split(',').map(Number)
                  );
                }
              },
              {
                noAck: true,
              }
            );
          }
        );
      });
    });
  });
}

client();
