package ba.unsa.etf.rpr;

import java.util.*;

public class Semestar {
    private String naziv;
    private int ectsObavezni;
    private List<Predmet> obavezniPredmeti;
    private List<Predmet> izborniPredmeti;

    public Semestar(String naziv) {
        if(naziv.isBlank()) throw new IllegalArgumentException("Naziv je neispravan!");
        this.naziv = naziv;
        ectsObavezni=0;
        obavezniPredmeti=new ArrayList<>();
        izborniPredmeti=new ArrayList<>();
    }

    public String getNaziv() {
        return naziv;
    }

    public void setNaziv(String naziv) {
        if(naziv.isBlank()) throw new IllegalArgumentException("Naziv je neispravan!");
        this.naziv = naziv;
    }

    public int getEctsObavezni() {
        return ectsObavezni;
    }

    public List<Predmet> getObavezniPredmeti() {
        return obavezniPredmeti;
    }

    public void dodajObavezanPredmet(Predmet predmet) {
        if(predmet==null) throw new IllegalArgumentException("Predmet je neispravan!");
        if(obavezniPredmeti.contains(predmet) || izborniPredmeti.contains(predmet)) throw new IllegalArgumentException("Predmet je već u semestru!");
        obavezniPredmeti.add(predmet);
        ectsObavezni=ectsObavezni+predmet.getEcts();
    }

    public void izbaciObavezanPredmet(Predmet predmet) {
        if(predmet==null) throw new IllegalArgumentException("Predmet je neispravan!");
        if(obavezniPredmeti.remove(predmet)) throw new IllegalArgumentException("Predmet nije u semestru!");
        ectsObavezni=ectsObavezni-predmet.getEcts();
    }

    public List<Predmet> getIzborniPredmeti() {
        return izborniPredmeti;
    }

    public void dodajIzborniPredmet(Predmet predmet) {
        if(predmet==null) throw new IllegalArgumentException("Predmet je neispravan!");
        if(obavezniPredmeti.contains(predmet) || izborniPredmeti.contains(predmet)) throw new IllegalArgumentException("Predmet je već u semestru!");
        izborniPredmeti.add(predmet);
    }

    public void izbaciIzborniPredmet(Predmet predmet) {
        if(predmet==null) throw new IllegalArgumentException("Predmet je neispravan!");
        if(izborniPredmeti.remove(predmet)) throw new IllegalArgumentException("Predmet nije u semestru!");
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        Semestar semestar = (Semestar) o;
        return ectsObavezni == semestar.ectsObavezni && naziv.equals(semestar.naziv) && Objects.equals(obavezniPredmeti, semestar.obavezniPredmeti) && Objects.equals(izborniPredmeti, semestar.izborniPredmeti);
    }

    @Override
    public int hashCode() {
        return Objects.hash(naziv);
    }

}
