package ba.unsa.etf.rpr;

import java.util.Objects;

public class Odgovor{
    private String tekstOdgovora;
    private boolean tacno;

    public Odgovor(String tekstOdgovora, boolean tacno) {
        this.tekstOdgovora = tekstOdgovora;
        this.tacno = tacno;
    }

    public String getTekstOdgovora() {
        return tekstOdgovora;
    }

    public void setTekstOdgovora(String tekstOdgovora) {
        this.tekstOdgovora = tekstOdgovora;
    }

    public boolean isTacno() {
        return tacno;
    }

    public void setTacno(boolean tacno) {
        this.tacno = tacno;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        Odgovor odgovor = (Odgovor) o;
        return tacno == odgovor.tacno && tekstOdgovora.equals(odgovor.tekstOdgovora);
    }

    @Override
    public int hashCode() {
        return Objects.hash(tekstOdgovora, tacno);
    }
}
