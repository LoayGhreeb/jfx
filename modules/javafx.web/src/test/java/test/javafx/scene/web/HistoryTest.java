/*
 * Copyright (c) 2011, 2024, Oracle and/or its affiliates. All rights reserved.
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * This code is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 only, as
 * published by the Free Software Foundation.  Oracle designates this
 * particular file as subject to the "Classpath" exception as provided
 * by Oracle in the LICENSE file that accompanied this code.
 *
 * This code is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * version 2 for more details (a copy is included in the LICENSE file that
 * accompanied this code).
 *
 * You should have received a copy of the GNU General Public License version
 * 2 along with this work; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 * Please contact Oracle, 500 Oracle Parkway, Redwood Shores, CA 94065 USA
 * or visit www.oracle.com if you need additional information or have any
 * questions.
 */

package test.javafx.scene.web;

import java.util.concurrent.atomic.AtomicBoolean;
import java.io.File;
import javafx.beans.value.ObservableValue;
import javafx.scene.web.WebHistory;
import javafx.beans.value.ChangeListener;
import javafx.collections.ListChangeListener;
import java.util.Date;

import org.junit.jupiter.api.Test;
import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertTrue;
import static org.junit.jupiter.api.Assertions.fail;

public class HistoryTest extends TestBase {
    WebHistory history = getEngine().getHistory();

    AtomicBoolean entriesChanged = new AtomicBoolean(false);
    AtomicBoolean titleChanged = new AtomicBoolean(false);
    AtomicBoolean dateChanged = new AtomicBoolean(false);
    AtomicBoolean indexChanged = new AtomicBoolean(false);

    @Test public void test() {

        //
        // before history is populated
        //
        submit(() -> {
            try {
                history.go(-1);
                fail("go: IndexOutOfBoundsException is not thrown");
            } catch (IndexOutOfBoundsException ex) {}
            try {
                history.go(1);
                fail("go: IndexOutOfBoundsException is not thrown");
            } catch (IndexOutOfBoundsException ex) {}

            history.setMaxSize(99);
            assertEquals(history.getMaxSize(), 99, "max size is wrong");
        });

        // [1*]
        checkLoad(new File("src/test/resources/test/html/h1.html"), 1, 0, "1");

        // [1, 2*]
        checkLoad(new File("src/test/resources/test/html/h2.html"), 2, 1, "2");

        //
        // check the list update
        //
        history.getEntries().addListener(new ListChangeListener<WebHistory.Entry>() {
            @Override
            public void onChanged(ListChangeListener.Change<? extends WebHistory.Entry> c) {
                c.next();
                assertTrue(c.wasAdded(), "entries: change is wrong");
                assertTrue(c.getAddedSubList().size() == 1, "entries: size is wrong");
                history.getEntries().removeListener(this);
                entriesChanged.set(true);
            }
        });

        // [1, 2, 3*]
        checkLoad(new File("src/test/resources/test/html/h3.html"), 3, 2, "3");

        ensureValueChanged(entriesChanged, "entries not changed after load");

        /*
        This commented code block causes test failure after JDK-8268849.
        An issue is raised: JDK-8269912, to investigate the failure.

        //
        // check the title update
        //
        history.getEntries().get(history.getCurrentIndex()).titleProperty().addListener(new ChangeListener<String>() {
            public void changed(ObservableValue<? extends String> observable, String oldValue, String newValue) {
                assertEquals(oldValue, "entries: old title is wrong", "3");
                assertEquals(newValue, "entries: new title is wrong", "hello");
                observable.removeListener(this);
                titleChanged.set(true);
            }
        });
        executeScript("document.title='hello'");

        ensureValueChanged(titleChanged, "title not changed from JS");
        */

        //
        // check the date & index updates
        //
        history.getEntries().get(history.getCurrentIndex() - 1).lastVisitedDateProperty().addListener(newDateListener());

        try {
            Thread.sleep(150); // ensure the next date doesn't fit into the same millisecond
        } catch (InterruptedException e) {
            fail(e);
        }

        history.currentIndexProperty().addListener(new ChangeListener<Number>() {
            @Override
            public void changed(ObservableValue<? extends java.lang.Number> observable, Number oldValue, Number newValue) {
                assertEquals(2, oldValue, "currentIndexProperty: old index is wrong");
                assertEquals(1, newValue, "currentIndexProperty: new index is wrong");
                observable.removeListener(this);
                indexChanged.set(true);
            }
        });

        submit(() -> {
            // [1, 2*, 3]
            history.go(-1);
        });
        waitLoadFinished();
        check(new File("src/test/resources/test/html/h2.html"), 3, 1, "2");

        ensureValueChanged(dateChanged, "date not changed after go(-1)");
        ensureValueChanged(indexChanged, "index not changed after go(-1)");

        //
        // more go() checks
        //

        submit(() -> {
            // [1, 2, 3*]
            history.go(1);
        });
        waitLoadFinished();
        check(new File("src/test/resources/test/html/h3.html"), 3, 2, "3");

        submit(() -> {
            // [1*, 2, 3]
            history.go(-2);
        });
        waitLoadFinished();
        check(new File("src/test/resources/test/html/h1.html"), 3, 0, "1");

        submit(() -> {
            // [1*, 2, 3]
            history.go(0); // no-op
        });

        submit(() -> {
            // [1*, 2, 3]
            try {
                history.go(-1);
                fail("go: IndexOutOfBoundsException is not thrown");
            } catch (IndexOutOfBoundsException ex) {}
        });

        submit(() -> {
            // [1, 2, 3*]
            history.go(2);
        });
        waitLoadFinished();
        check(new File("src/test/resources/test/html/h3.html"), 3, 2, "3");

        submit(() -> {
            // [1, 2, 3*]
            try {
                history.go(1);
                fail("go: IndexOutOfBoundsException is not thrown");
            } catch (IndexOutOfBoundsException ex) {}
        });

        //
        // check the maxSize
        //

        submit(() -> {
            // [1, 2, 3*]
            history.setMaxSize(3);
        });
        // [2, 3, 1*]
        checkLoad(new File("src/test/resources/test/html/h1.html"), 3, 2, "1");

        submit(() -> {
            // [2, 3*]
            history.setMaxSize(2);
            assertEquals(2, history.getEntries().size(), "entries: size is wrong");
            //JDK-8335596
            //assertEquals(history.getEntries().get(0).getTitle(), "entries: title is wrong", "2");
        });

        submit(() -> {
            // [2, 3*]
            history.setMaxSize(3);
            // [2*, 3]
            history.go(-1);
        });
        waitLoadFinished();

        // [2, 1*]
        checkLoad(new File("src/test/resources/test/html/h1.html"), 2, 1, "1");
        // [2, 1, 3*]
        checkLoad(new File("src/test/resources/test/html/h3.html"), 3, 2, "3");

        submit(() -> {
            // [2*, 1, 3]
            history.go(-2);
        });
        waitLoadFinished();

        //
        // check for load in-beetwen
        //

        // [2, 3*]
        checkLoad(new File("src/test/resources/test/html/h3.html"), 2, 1, "3");

        //
        // check the date update on reload
        //

        // [2, 3, 4*]
        load(new File("src/test/resources/test/html/h4.html"));

        history.getEntries().get(history.getCurrentIndex()).lastVisitedDateProperty().addListener(newDateListener());

        try {
            Thread.sleep(150); // ensure the next date doesn't fit into the same millisecond
        } catch (InterruptedException e) {
            fail(e);
        }

        reload();

        ensureValueChanged(dateChanged, "date not changed after reload");

        //
        // finally, check zero and invalid maxSize
        //

        submit(() -> {
            // []
            history.setMaxSize(0);
            assertEquals(0, history.getEntries().size(), "maxSizeProperty: wrong value");

            // []
            try {
                history.maxSizeProperty().set(-1);
                fail("maxSizeProperty: IllegalArgumentException is not thrown");
            } catch (IllegalArgumentException ex) {}
        });
    }

    void checkLoad(File file, int size, int index, String title) {
        load(file);
        check(file, size, index, title);
    }

    void check(File file, int size, int index, String title) {
        assertEquals(size, history.getEntries().size(), "entries: size is wrong");
        assertEquals(index, history.getCurrentIndex(), "currentIndex: index is wrong");
        assertEquals(file.toURI().toString(), history.getEntries().get(index).getUrl(), "entries: url is wrong");
        /*
        The following assert causes test failure after JDK-8268849.
        An issue is raised: JDK-8269912, to investigate the failure.
        // assertEquals(title, history.getEntries().get(index).getTitle(), "entries: title is wrong");
        */
    }

    void ensureValueChanged(AtomicBoolean value, String errMsg) {
        if (!value.compareAndSet(true, false)) {
            fail(errMsg);
        }
    }

    ChangeListener newDateListener() {
        return new ChangeListener<Date>() {
            long startTime = System.currentTimeMillis();

            @Override
            public void changed(ObservableValue<? extends Date> observable, Date oldValue, Date newValue) {
                long curTime = System.currentTimeMillis();

                if (newValue.before(oldValue) ||
                        newValue.getTime() < startTime ||
                        newValue.getTime() > curTime)
                {
                    System.out.println("oldValue=" + oldValue.getTime() +
                            ", newValue=" + newValue.getTime() +
                            ", startTime=" + startTime +
                            ", curTime=" + curTime);

                    fail("entries: date is wrong");
                }
                observable.removeListener(this);
                dateChanged.set(true);
            }
        };

    }
}
