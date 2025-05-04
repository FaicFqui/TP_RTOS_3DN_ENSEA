# TP_RTOS_3DN_ENSEA

Clignotement led sans FreeRtos et avec FreeRTOS est fonctionnel

Source de temps pour FreeRTOS est TIM6

Manipulation des sémaphores, notifications, queues est effectuée.

Solution sémaphore Mutex proposé pour proteger l'accès à la fonction task_bug

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

Intégration des fonctions shell_run()/shell_init()/shell_add() dans une tache RTOS crée dans le main :
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
                                               
> h
                                                                           
h: Help
                                                                       
f: Une fonction inutile
                                                       
a: Addition de deux entiers
                                                   
l: Allumer LED (GPIOI_PIN1)
                                                   
> f
                                                                           
Je suis une fonction bidon
                                                    
Nombre d'arguments : 1
                                                        
Arg[0] = f
                                                                    
> a 1 99
                                                                      
Addition : 1 + 99 = 100
                                                       
> l
                                                                           
LED PIN1 GPIOI est allumÃ©e !
                                                 
> 

 
