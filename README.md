# Gerçek Zamanlı Sohbet Programı (ft_irc)

## Genel Bakış

ft_irc, C++ ile yazılmış ve kullanıcıların bir sunucuya bağlanarak birbirleriyle gerçek zamanlı iletişim kurmasını sağlayan bir gerçek zamanlı sohbet programıdır. Bu program, iletişimi ve gerçek zamanlı güncellemeleri sağlamak için soket ve poll kütüphanelerini ve C++ class,map,vector yapılarını kullanır.

## Özellikler

- Birden fazla kullanıcı, merkezi bir sunucuya bağlanabilir ve sohbet odalarına katılabilir.
- Kullanıcılar gerçek zamanlı mesajlar gönderebilir ve alabilir.
- Sunucu, farklı sohbet odalarını yönetebilir.
- Temel kullanıcı kimlik doğrulama desteği sağlar.
- Basit ve verimli bir tasarıma sahiptir.

## Gereksinimler

Sohbet programını çalıştırmadan önce, aşağıdaki bağımlılıklara ihtiyacınız vardır:

- C++ derleyici (örneğin, g++)
- Soket ve poll kütüphaneleri

## Başlangıç

1. Deposunu klonlayın:

`git clone https://github.com/ozermuharrem/ft_irc.git`

2. Sunucu ve istemci programlarını derleyin:

  `cd ft_irc`
  <br>
  `make re`
  
3. Sunucuyu başlatın:

`./ircserver <port> <password>`

4. Yeni bir terminalde bir istemci başlatın ve sunucuya bağlanın:

`nc 127.0.0.1 <port> `

5. Diğer bağlı kullanıcılarla sohbet etmeye başlayabilmek için şifre girmeli ve yeni bir kullanıcı oluşturmalısın Sırası ile komutları gir.

  `cap` 
  <br>
  `pass <password>`
  <br>
  `user <userName>:<userName>:<userName>:<userName>`
  <br>
  `nick <userNickName>`

6.  Artık diğer kullanıcılırla sohbet edebilirsin
  
  `privmsg <userName> <mesaj>`
  
7.  Kanal oluşturup grup mesajı da iletebilirsin
   
  `join #<kanalName>` kanal oluşturmak için <br>
  
  `privmsg #<kanalName> <mesaj>`
  
8.  Kanala limit atama Kick gibi birden fazla mod bulunmaktadır. 

## Kullanım

- Sunucuya bağlanmak için, istemci uygulamasını çalıştırın ve sunucunun IP adresini ve port numarasını girin.
- Bağlandıktan sonra sohbet odalarına katılabilir ve mesaj gönderip alabilirsiniz.

## Katkıda Bulunma

Sohbet programını geliştirmek için katkılara açığız. Aşağıdakileri yaparak katkıda bulunabilirsiniz:

- Hataları veya sorunları bildirme.
- Yeni özellikler veya geliştirmeler önerme.
- Pull talepleri gönderme.

## İletişim

Herhangi bir sorunuz veya daha fazla yardıma ihtiyacınız varsa, lütfen [muharrem.o@outlook.com] adresi ile iletişime geçin.

Keyifli Sohbetler!
