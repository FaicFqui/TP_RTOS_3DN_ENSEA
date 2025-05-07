# TP_RTOS_3DN_ENSEA (FQUIHI Faiçal / HAJJAJI Adnane)

# 1ère Partie : tâches et sémaphores

- Clignotement led sans FreeRtos et avec FreeRTOS est fonctionnel.

- Source de temps pour FreeRTOS est TIM6.

- Manipulation des sémaphores, notifications, queues est effectuée.

- Solution sémaphore Mutex proposé pour proteger l'accès à la fonction task_bug.


# 2èmme Partie : SHELL

//////////////////// ************ Shell en FREERTOS ********************* //////////

Compilation et intégration du shell sans son utilisation dans une tache FreeRTOS est fonctionnel :
affichage console RealTerm :

===== Monsieur Shell v0.2 =====
                                               
> ff
                                                                          
                                                                           
Je suis une fonction bidon

*****************
Chaque unité dev citée au-dessus est tracée par des commit sur le repo GitHub.



Dans FreeRTOSConfig.h : #define configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY 5

Création de paire drv_uart.c et .h dans lesquelles on délègue une partie du travail d'abstraction matérielle UART qui était fait dasn shell.c. à savoir : 
Transmission uart - réception uart en interruption

Intégration des fonctions shell_run()/shell_init()/shell_add() dans une tache RTOS créée dans le main :
instnace globale : h_shell_t shell

void ShellTask(void *argument) {
    shell.io.drv_shell_transmit = drv_uart_transmit;
    shell.io.drv_shell_receive  = drv_uart_receive;

    drv_uart_init_IT();  // // Lancement de la première réception en IT

    shell_init(&shell);
    shell_add(&shell, 'f', fonction, "Une fonction inutile");
    shell_add(&shell, 'a', addition, "Addition de deux entiers");
    shell_add(&shell, 'l', cmd_led_on, "Allumer LED (GPIOI_PIN1)");

    shell_run(&shell);  // éxécute une boucle while(1)
}

xTaskCreate(ShellTask, "Shell", 512, NULL, 1, NULL);

Après exécution du programme, terminal Realterm :
                                                                                
===== Monsieur Shell v0.2 =====
                                               
> f
                                                                           
Je suis une fonction bidon
                                                    
Nombre d'arguments : 1
                                                        
Arg[0] = f
                                                                    
> a 1 99
                                                                      
Addition : 1 + 99 = 100
                                                       
> l
                                                                           
LED PIN1 GPIOI est allumee !
                                                  
> e
                                                                           
LED PIN1 GPIOI est eteinte !
                                                  
>       

# 3èmme Partie : Debug, gestion d'erreur et statistiques

///////////////////************** Gestion mémoire TAS ET PILE ***************///////////////

les commandes arm-none-eabi-size et arm-none-eabi-objdump :
```bash
arm-none-eabi-size TP_RTOS.elf
arm-none-eabi-objdump -h -S TP_RTOS.elf > "TP_RTOS.list"

   text	   data	    bss	    dec	    hex	filename
  33724	    108	  21028	  54860	   d64c	TP_RTOS.elf
Finished building: default.size.stdout

Explication :

- `text` : Code exécutable + constantes (`.text`, `.rodata`) → en **FLASH/ROM**
- `data` : Variables initialisées (`.data`) → en **RAM**
- `bss`  : Variables non initialisées (`.bss`) → en **RAM**
- `dec`  : Total en **décimal** = `text + data + bss` = **54860 octets**

FLASH (ROM) :
text + data = 33724 + 108 = 33832 octets

Donc code occupe à peu près 33 Ko de Flash

RAM :
data + bss = 108 + 21028 = 21136 octets

Donc à peu près 21 Ko de RAM statique pour faire fonctionner le code.

Cela n’inclut pas la heap ni la pile des tâches allouées dynamiquement.


- Taille du tas pour FreeRTOS : 15360 octets
- "Pile d'une tache : 12 words × 4 octets/word = 2048 octets"

- consommation totale = 15360 - 13120 = 2240 octets
- Pile = 2048 octets
----> Donc TCB = 2240 - 2048 = 192 octets

Nombre totale de taches possible à créer avec 15360 ocets du tas pour FREERTOS :
15360 Division entière 2240 = 6 taches possibles.

Résulats sur Realterm :

Tache 1 creee avec succes | Heap restant : 13120 octets
                       
Tache 2 creee avec succes | Heap restant : 10888 octets
                                              
Tache 3 creee avec succes | Heap restant : 8656 octets
                                                                      
Tache 4 creee avec succes | Heap restant : 6424 octets
              
Tache 5 creee avec succes | Heap restant : 4192 octets
                                      
Tache 6 creee avec succes | Heap restant : 1960 octets
                                                              
Tache 7 : echec de creation (peut etre memoire epuisee) 

////////***** Fin affichage ***************////////
  
Gestion de la Pile

Explication de la documentation

#define configCHECK_FOR_STACK_OVERFLOW x

x = 0 : Aucune vérification
x = 1 : Vérifie une valeur sentinelle en haut de la pile
x = 2 : Vérifie aussi les débordements de pile vers le bas

En cas de dépassement de la pile d'une tâche l'OS appelle automatiquement la foncion : 
vApplicationStackOverflowHook

xTaskCreate(TacheOverflow, "OVERFLOW", 128, NULL, 1, NULL) : 128 mots = 128 *4 = 512 ocets.

Résulat en entrant dans la tâche overflow avec  : 

UBaseType_t remaining = uxTaskGetStackHighWaterMark(NULL);

printf("Pile restante avant recursion : %lu mots (%lu octets)\n",remaining, remaining * sizeof(StackType_t));

Sur Realterm " Pile restante avant recursion : 67 mots (268 octets) "

Il reste 67 mots = 268 octets de pile disponibles,mais comme chaque appel récursif consomme environ 128 octets, 
le dépassement de la pile aura lieu au 2ᵉ ou 3ᵉ appel récursif.

/////////*********************** Statistique dans "l'IDE" ***************************////////
 COnfiguration dans FreeRTOSConfig.h
 
#define configGENERATE_RUN_TIME_STATS           1
#define configUSE_STATS_FORMATTING_FUNCTIONS    1
#define configUSE_TRACE_FACILITY                1

#define portCONFIGURE_TIMER_FOR_RUN_TIME_STATS() configureTimerForRunTimeStats()
#define portGET_RUN_TIME_COUNTER_VALUE()        getRunTimeCounterValue()

"Ajout d'un compteur dans cette fonction" :

<pre> ```c void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) { if (htim->Instance == TIM6) { HAL_IncTick(); // Incrémente le tick système (HAL) tim6_overflow_count++; // Compteur FreeRTOS pour les stats CPU } } ``` </pre>

et définition de deux fonctions :  
  
  void configureTimerForRunTimeStats(void) : Cette fonction est appelée automatiquement par FreeRTOS au démarrage, 
pour que tu puisses initialiser un timer matériel (ou autre base de temps) qui sera utilisé comme source de mesure.

unsigned long getRunTimeCounterValue(void) : 
Cette fonction est appelée fréquemment par FreeRTOS pour lire la valeur actuelle du "compteur de temps". 
Cette valeur est utilisée pour calculer :

	1.combien de "temps" une tâche a passé en cours d’exécution
	2.le pourcentage d’utilisation CPU (dans vTaskGetRunTimeStats())
	
Affichage statistiques depuis une tâche indépendante, résultat sur RealTerm :

 === RunTime Stats ===
                                                         
                       TacheStats     	 13568775	       	       <1%
           
IDLE           	        2228154574	     	       99%
                           
Tmr Svc        	        848	    	       <1%
                                   
Shell          	        196870	 	       <1%

Affichage statistiques depuis La tache Shell, résultat sur RealTerm :   

===== Monsieur Shell v0.2 =====
                                               
> l
                                                                           
LED PIN1 GPIOI est allumee !
                                                  
> s
                                                                           
=== RunTime Stats ===
                                                          
                      Shell          	  458460	 	       <1%
                   
IDLE           	        824377656	      	       99%
                           
Tmr Svc        	        757	    	       <1%
  
 






