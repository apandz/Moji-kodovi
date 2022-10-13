package ba.unsa.etf.rpr;

import java.util.Map;
import java.util.Objects;

public class RezultatKviza {
    private Kviz kviz;
    private double total;
    private Map<Pitanje,Double> bodovi;

    public RezultatKviza(Kviz kviz) {
        this.kviz = kviz;
        total=0;
    }

    public Kviz getKviz() {
        return kviz;
    }

    public void setKviz(Kviz kviz) {
        this.kviz = kviz;
    }

    public double getTotal() {
        return total;
    }

    public void setTotal(double total) {
        this.total = total;
    }

    public Map<Pitanje, Double> getBodovi() {
        return bodovi;
    }

    public void setBodovi(Map<Pitanje, Double> bodovi) {
        this.bodovi = bodovi;
    }

    @Override
    public String toString() {
        StringBuilder rezultatKviza=new StringBuilder("Na kvizu \""+kviz.getNaziv()+"\" ostvarili ste ukupno "+total+" poena. Raspored po pitanjima:");
        for(var entry: bodovi.entrySet())
            rezultatKviza.append("\n").append(entry.getKey().getTekst()).append(" - ").append(entry.getValue().toString()).append("b");
        return rezultatKviza.toString();
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        RezultatKviza that = (RezultatKviza) o;
        return Double.compare(that.total, total) == 0 && kviz.equals(that.kviz) && Objects.equals(bodovi, that.bodovi);
    }

    @Override
    public int hashCode() {
        return Objects.hash(kviz, total, bodovi);
    }
}
