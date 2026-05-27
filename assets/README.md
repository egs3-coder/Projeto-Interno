# Assets opcionais da GUI

Esta pasta pode receber imagens locais para a versao `jogo_gui.exe`.

O jogo continua funcionando sem esses arquivos, usando os desenhos feitos em `raylib`.
Agora a GUI tambem aproveita automaticamente os spritesheets prontos desta pasta como fallback:

- `8BitDeck.png` para as 52 cartas do baralho
- `Jokers.png` para os Coringas
- `Tarots.png` para as cartas Tarot
- `Vouchers.png` para os cupons da loja
- `boosters.png` para os pacotes

As imagens individuais abaixo continuam sendo suportadas e, quando existem, tem prioridade sobre o spritesheet correspondente.

## Estrutura esperada

```text
assets/
  cards/A_copas.png
  cards/2_copas.png
  cards/K_paus.png
  enhancements/1.png
  enhancements/2.png
  enhancements/3.png
  enhancements/4.png
  enhancements/5.png
  jokers/1.png
  editions/1.png
  editions/2.png
  editions/3.png
  editions/4.png
  tarots/1.png
  coupons/0.png
  packs/0.png
  villain/jimbo.png
```

## Mapeamento rapido

- `cards/<rank>_<naipe>.png`: ranks `A`, `2`...`10`, `J`, `Q`, `K`; naipes `copas`, `ouros`, `espadas`, `paus`
- `enhancements/1..5.png`: bonus, mult, sorte, vidro e carta coringa
- `editions/1..4.png`: holografica/foil, cromada, prismatica e negativa
- `jokers/<id>.png`: usa os ids do enum `JokerType` em `src/shop.h`
- `tarots/<id>.png`: usa os ids do enum `TarotType` em `src/shop.h`
- `coupons/<id>.png`: usa os ids do enum `CouponType` em `src/shop.h`
- `packs/0..5.png`: usa os ids de `PackKind` em `src/raylib_main.c`

Use apenas arquivos que voce tem permissao para usar no contexto da apresentacao.
