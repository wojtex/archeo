#include "common.h"
#include "debug.h"
#include "double_queue.h"

#include <stdio.h>
#include <stdlib.h>

static int *Teren;
static int *Szacunek;
static int OplataStala;
static int OgraniczenieArtefaktow;
static int Dlugosc;
static int Glebokosc;
static int *Kolekcje;
static int LiczbaFirm;


static double_queue_t mqueue;

static int * Rezerwacje;

static int *ChceGadac;

void cleanup() {
  bold("Running cleanup...");
  double_queue_close(&mqueue);
}

int czekaj_na_polaczenie_z_bankiem(
      void *query, size_t query_length,
      void **response, size_t *response_length,
      int * is_notification,
      int source) {
  if(source != bank_ip) {
    warning("Wrong sender address: source = %x", source);
    //return -1;
  }
  if(query_length != 1 + sizeof(int) || ((char*)query)[0] != ACT_BANK_CONNECT) {
    error("Expected BANK CONNECT message: command = %x", ((char*)query)[0]);
    //return -1;
  }
  memcpy(&LiczbaFirm, query + 1, sizeof(int));
  char *resp = malloc(1);
  resp[0] = ACT_OK;
  (*response) = resp;
  (*response_length) = 1;
  (*is_notification) = 0;
  return 0;
}

int main(int argc, const char *argv[]) {
  bold("Starting museum process.");
  if(argc != 5) {
    error("Wrong command-line input - four parameters needed.");
    return 1;
  }
  if(sscanf(argv[1], "%d", &Dlugosc) != 1) {
    error("First parameter should be a number.");
    return 1;
  }
  if(sscanf(argv[2], "%d", &Glebokosc) != 1) {
    error("Second parameter should be a number.");
    return 1;
  }
  if(sscanf(argv[3], "%d", &OplataStala) != 1) {
    error("Third parameter should be a number.");
    return 1;
  }
  if(sscanf(argv[4], "%d", &OgraniczenieArtefaktow) != 1) {
    error("Fourth parameter should be a number.");
    return 1;
  } else {
    OgraniczenieArtefaktow++; // bo chcemy większe (>=), a nie ściśle większe (>)
  }

  //
  // Create message queues
  //
  log("Creating message queues for museum.");

	if(double_queue_init(&mqueue, key_input, key_output, 0700|IPC_CREAT, 0700|IPC_CREAT) == -1) {
		error("Failed creating message queue.");
		cleanup();
		return 1;
	}

  //
  // Read data
  //
  log("Reading data from stdin");
  if((Teren = malloc(Dlugosc*Glebokosc*sizeof(int))) == NULL) {
    fprintf(stderr, "Allocation failed.");
    cleanup();
    return 1;
  }
  if((Szacunek = malloc(Dlugosc*Glebokosc*sizeof(int))) == NULL) {
    fprintf(stderr, "Allocation failed.");
    cleanup();
    return 1;
  }
  if((Kolekcje = malloc(OgraniczenieArtefaktow*sizeof(int))) == NULL) {
    fprintf(stderr, "Allocation failed.");
    cleanup();
    return 1;
  } else {
    for(int i = 0; i < OgraniczenieArtefaktow; i++) {
      Kolekcje[i] = 0;
    }
  }
  if((Rezerwacje = malloc(Dlugosc*sizeof(int))) == NULL) {
    fprintf(stderr, "Allocation failed.");
    cleanup();
    return 1;
  } else {
    for(int i = 0; i < Dlugosc; i++) {
      Rezerwacje[i] = 0;
    }
  }

  for(int i = 0; i < Dlugosc; i++) {
    for(int j = 0; j < Glebokosc; j++) {
      scanf("%d", Teren+i*Glebokosc+j);
    }
  }

  for(int i = 0; i < Dlugosc; i++) {
    for(int j = 0; j < Glebokosc; j++) {
      scanf("%d", Szacunek+i*Glebokosc+j);
    }
  }

  //
  // Słuchaj, kto chce gadać i rozmawiaj...
  //
  bold("Waiting for bank connection...");
  if(double_queue_listen(&mqueue, czekaj_na_polaczenie_z_bankiem, museum_ip) == -1) {
    error("Listening error during waiting for bank connection.");
    cleanup();
    return 1;
  }
  bold("Listening for queries...");
  // TODO
  cleanup();
}

// L - lewa pozycja
// P - prawa pozycja
// G - głębokość
void WycenTeren(int L, int P, int G) {
  // TODO
}

// k - liczba pracowników
// Z - cena
void WydajPozwolenieKopania(int k, int Z) {
  // TODO
}

void KupKolekcje() {
  // TODO
}

void RaportMuzeum() {
  for(int i = 0; i < Dlugosc; i++) {
    int depth = 0;
    for(int j = 0; j < Glebokosc; j++) {
      int r = 0;
      if(Teren[i * Glebokosc + j] == 0) r = 0;
      else {
        if(Rezerwacje[i] > depth) r = 1;
        else r = 2;
        depth++;
      }
      printf("%d", r);
    }
    printf("\n");
  }

  // TODO: Raporty firm tutaj?
}

void Koncz() {
  // TODO
}

void OnSigINT() {
  // TODO
}
