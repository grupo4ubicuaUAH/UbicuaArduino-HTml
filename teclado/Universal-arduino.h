/ *
Derechos de autor (c) 2018 Brian Lough. Todos los derechos reservados.
UniversalTelegramBot - Biblioteca para crear su propio Telegram Bot usando
ESP8266 o ESP32 en Arduino IDE.
Esta biblioteca es software gratuito; puedes redistribuirlo y / o
modificarlo bajo los términos del Público General Menor de GNU
Licencia publicada por la Free Software Foundation; ya sea
versión 2.1 de la Licencia, o (a su elección) cualquier versión posterior.
Esta biblioteca se distribuye con la esperanza de que sea útil,
pero SIN NINGUNA GARANTÍA; sin siquiera la garantía implícita de
COMERCIABILIDAD o APTITUD PARA UN PROPÓSITO PARTICULAR. Ver el GNU
Licencia pública general menor para obtener más detalles.
Debería haber recibido una copia de GNU Lesser General Public
Licencia junto con esta biblioteca; si no, escriba al software libre
Foundation, Inc., 51 Franklin St, quinto piso, Boston, MA 02110-1301 EE. UU.
* /

# ifndef UniversalTelegramBot_h
# define  UniversalTelegramBot_h

// #define TELEGRAM_DEBUG 1
# define  ARDUINOJSON_DECODE_UNICODE  1
# define  ARDUINOJSON_USE_LONG_LONG  1
# incluir  < Arduino.h >
# include  < ArduinoJson.h >
# incluye  < Client.h >
# include  < TelegramCertificate.h >

# define  TELEGRAM_HOST  " api.telegram.org "
# define  TELEGRAM_SSL_PORT  443
# define  HANDLE_MESSAGES  1

// desmarca la siguiente línea para habilitar el modo de depuración
// #define _debug

typedef  bool (* Más datos disponibles) ();
typedef  byte (* GetNextByte) ();
typedef byte * (* GetNextBuffer) ();
typedef  int (GetNextBufferLen) ();

struct  telegramMessage {
  Texto de cadena;
  String chat_id;
  String chat_title;
  String from_id;
  String from_name;
  Fecha de cadena;
  Tipo de cadena;
  String file_caption;
  String file_path;
  String file_name;
  bool hasDocument;
  long file_size;
  longitud de flotación ;
  latitud de flotación ;
  int update_id;
  int message_id;  

  int reply_to_message_id;
  String reply_to_text;
  String query_id;
};

class  UniversalTelegramBot {
público:
  UniversalTelegramBot ( cadena constante y token, cliente y cliente);
  void  updateToken ( const String & token);
  String getToken ();
  String sendGetToTelegram ( const String & command);
  String sendPostToTelegram ( const String & command, carga útil de JsonObject);
  Cuerda
  sendMultipartFormDataToTelegram ( const String & command, const String & binaryPropertyName,
                                  const String & fileName, const String & contentType,
                                  const String & chat_id, int fileSize,
                                  MoreDataAvailable moreDataAvailableCallback,
                                  GetNextByte getNextByteCallback, 
                                  GetNextBuffer getNextBufferCallback, 
                                  GetNextBufferLen getNextBufferLenCallback);

  bool  readHTTPAnswer (Cadena y cuerpo, Cadena y encabezados);
  bool  getMe ();

  bool  sendSimpleMessage ( const String & chat_id, const String & text, const String & parse_mode);
  bool  sendMessage ( const String & chat_id, const String & text, const String & parse_mode = " " , int message_id = 0 );
  bool  sendMessageWithReplyKeyboard ( const String & chat_id, const String & text,
                                    const String y parse_mode, const String y teclado,
                                    bool cambio de tamaño = falso , bool Onetime = falso ,
                                    bool selectivo = falso );
  bool  sendMessageWithInlineKeyboard ( const String & chat_id, const String & text,
                                     const String & parse_mode, const String & keyboard, int message_id = 0 );

  bool  sendChatAction ( const String & chat_id, const String & text);

  bool  sendPostMessage (carga útil de JsonObject, bool edit = false );
  String sendPostPhoto (carga útil de JsonObject);
  String sendPhotoByBinary ( const String & chat_id, const String & contentType, int fileSize,
                           MoreDataAvailable moreDataAvailableCallback,
                           GetNextByte getNextByteCallback, 
                           GetNextBuffer getNextBufferCallback, 
                           GetNextBufferLen getNextBufferLenCallback);
  String sendPhoto ( const String & chat_id, const String & photo, const String & caption = " " ,
                   bool disable_notification = false ,
                   int reply_to_message_id = 0 , const String & keyboard = " " );

  bool  answerCallbackQuery ( const String & query_id,
                           const String & text = " " ,
                           bool show_alert = falso ,
                           const String & url = " " ,
                           int tiempo_caché = 0 );

  bool  setMyCommands ( const String & commandArray);

  String buildCommand ( const String & cmd);

  int  getUpdates ( desplazamiento largo );
  bool  checkForOkResponse ( cadena constante y respuesta);
  mensajes de telegramMessage [HANDLE_MESSAGES];
  long last_message_received;
  Nombre de cadena;
  String userName;
  int longPoll = 0 ;
  unsigned  int waitForResponse = 1500 ;
  int _lastError;
  int last_sent_message_id = 0 ;
  int maxMessageLength = 1500 ;

privado:
  // JsonObject * parseUpdates (respuesta de cadena);
  String _token;
  Cliente * cliente;
  void  closeClient ();
  bool  getFile (String & file_path, long & file_size, const String & file_id);
  bool  processResult (resultado JSONObject, int messageIndex);
};

# endif