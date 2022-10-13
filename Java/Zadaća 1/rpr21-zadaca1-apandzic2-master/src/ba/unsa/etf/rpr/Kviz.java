package ba.unsa.etf.rpr;

import java.util.*;

public class Kviz {
    private String naziv;
    private List<Pitanje> pitanja;

    private SistemBodovanja sistemBodovanja;

    public String getNaziv() {
        return naziv;
    }

    public void setNaziv(String naziv) {
        this.naziv = naziv;
    }

    public List<Pitanje> getPitanja() {
        return pitanja;
    }

    public void setPitanja(List<Pitanje> pitanja) {
        this.pitanja = pitanja;
    }

    public SistemBodovanja getSistemBodovanja() {
        return sistemBodovanja;
    }

    public void setSistemBodovanja(SistemBodovanja sistemBodovanja) {
        this.sistemBodovanja = sistemBodovanja;
    }

    public Kviz(String naziv, SistemBodovanja sistemBodovanja) {
        this.naziv = naziv;
        this.sistemBodovanja = sistemBodovanja;
        pitanja = new ArrayList<>();
    }

    public void dodajPitanje(Pitanje pitanje) {
        for (Pitanje pitanje1 : pitanja)
            if (pitanje1.getTekst().equalsIgnoreCase(pitanje.getTekst()))
                throw new IllegalArgumentException("Ne možete dodati pitanje sa tekstom koji već postoji");
        pitanja.add(pitanje);
    }

    @Override
    public String toString() {
        StringBuilder kviz = new StringBuilder("Kviz \""+naziv+"\" boduje se ");
        if(sistemBodovanja==SistemBodovanja.PARCIJALNO_SA_NEGATIVNIM) kviz.append("parcijalno sa negativnim bodovima");
        else kviz.append(sistemBodovanja.toString().toLowerCase());
        kviz.append(". Pitanja:");
        for(int i=0; i<pitanja.size(); i++) {
            Pitanje pitanje = pitanja.get(i);
            kviz.append("\n").append(i + 1).append(". ").append(pitanje.getTekst()).append("(").append(pitanje.getBrojPoena()).append("b)");
            for(var entry: pitanje.getOdgovori().entrySet()) {
                kviz.append("\n\t").append(entry.getKey()).append(": ").append(entry.getValue().getTekstOdgovora());
                if(entry.getValue().isTacno())
                    kviz.append("(T)");
            }
            if(i!=pitanja.size()-1)kviz.append("\n");
        }
        return kviz.toString();
    }

    public RezultatKviza predajKviz(Map<Pitanje, ArrayList<String>> odabraniOdgovori) {
        RezultatKviza rezultatKviza=new RezultatKviza(this);
        Map<Pitanje,Double> bodovi = new HashMap<>();
        double total=0;
        for(var entry: odabraniOdgovori.entrySet()) {
            double brojBodova=entry.getKey().izracunajPoene(entry.getValue(), sistemBodovanja);
            bodovi.put(entry.getKey(), brojBodova);
            total=total+brojBodova;
        }
        for(Pitanje pitanje: pitanja)
            if(!bodovi.containsKey(pitanje)) bodovi.put(pitanje,0d);
        rezultatKviza.setBodovi(bodovi);
        rezultatKviza.setTotal(total);
        return rezultatKviza;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        Kviz kviz = (Kviz) o;
        return naziv.equals(kviz.naziv) && Objects.equals(pitanja, kviz.pitanja) && sistemBodovanja == kviz.sistemBodovanja;
    }

    @Override
    public int hashCode() {
        return Objects.hash(naziv, pitanja, sistemBodovanja);
    }
}
