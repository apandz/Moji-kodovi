package ba.unsa.etf.rpr;

import java.util.*;
import java.util.regex.Pattern;

public class Main {

    public static void main(String[] args) {
        System.out.println("Ugodno rješavanje zadaće 1 :)");
    }

    public static void igrajKviz(Kviz kviz) {
        System.out.println("Kviz \""+kviz.getNaziv()+"\":\n");
        Map<Pitanje,ArrayList<String>> mapOdabraniOdgovori = new HashMap<>();
        for(Pitanje pitanje: kviz.getPitanja()) {
            System.out.print(pitanje + "\n(Ako ima više odgovora navedite ih razdvojene znakom ',')\nOdgovor: ");
            Scanner scanner = new Scanner(System.in);
            String odgovor =scanner.nextLine();
            ArrayList<String> listOdabraniOdgovori=new ArrayList<>();
            if(!Objects.equals(odgovor, "")) {
                Pattern zarez = Pattern.compile(",");
                zarez.splitAsStream(odgovor).forEach(listOdabraniOdgovori::add);
            }
            mapOdabraniOdgovori.put(pitanje,listOdabraniOdgovori);
        }
        RezultatKviza rezultatKviza=kviz.predajKviz(mapOdabraniOdgovori);
        System.out.println(rezultatKviza);
    }
}
