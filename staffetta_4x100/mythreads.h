void Pthread_mutex_lock(pthread_mutex_t* AddrMutex){
    int rc = pthread_mutex_lock(AddrMutex);
    printf("aaaaaaaaaaaaaaaa\n");
    if(!rc){
        printf("Errore! Esco . \n");
        exit(1);
    }
}
    
