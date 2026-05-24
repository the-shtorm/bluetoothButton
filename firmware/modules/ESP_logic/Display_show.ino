extern SevSeg sevSeg; 

void Show_state(ProjectState state) {
    switch(state) {
        case STATE_FLASH: 
            sevSeg.setChars("FLSH");
            break;
        case STATE_KING:
            sevSeg.setChars("bInG");    
            break;
        case STATE_SANJI:
            sevSeg.setChars("5nJI");
            break;
        case STATE_RIGHT:
            sevSeg.setChars("rIHt");
            break;        
    }

    sevSeg.refreshDisplay();
}